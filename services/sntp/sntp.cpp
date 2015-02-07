// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			15.12.2014.
//! \note
// =============================================================================
#include "time.h"
#include "sntp.hpp"
#include "lwip/api.h"

PACK_STRUCT_BEGIN
struct sntp_msg {
	PACK_STRUCT_FIELD(u8_t           li_vn_mode				[1]);
	PACK_STRUCT_FIELD(u8_t           stratum				[1]);
	PACK_STRUCT_FIELD(u8_t           poll					[1]);
	PACK_STRUCT_FIELD(u8_t           precision				[1]);
	PACK_STRUCT_FIELD(u32_t          root_delay				[1]);
	PACK_STRUCT_FIELD(u32_t          root_dispersion		[1]);
	PACK_STRUCT_FIELD(u32_t          reference_identifier	[1]);
	PACK_STRUCT_FIELD(u32_t          reference_timestamp	[2]);
	PACK_STRUCT_FIELD(u32_t          originate_timestamp	[2]);
	PACK_STRUCT_FIELD(u32_t          receive_timestamp		[2]);
	PACK_STRUCT_FIELD(u32_t          transmit_timestamp		[2]);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

#define SNTP_LI_NO_WARNING          0x00

#define SNTP_VERSION                (4/* NTP Version 4*/<<3)

#define SNTP_MODE_MASK              0x07
#define SNTP_MODE_CLIENT            0x03
#define SNTP_MODE_SERVER            0x04
#define SNTP_MODE_BROADCAST         0x05

#define SNTP_STRATUM_KOD            0x00

#define DIFF_SEC_1900_1970         (2208988800UL)

using namespace communication;
using namespace service::sntp;
using namespace service::sntp::configuration;

sntp::sntp(): asbtractServiceRequestTransmitter(url, port, NETCONN_UDP, updatePeriode) {
	HibernateEnableExpClk(systemGlobal::currentSystemClockFrequency);
	HibernateCounterMode(HIBERNATE_COUNTER_24HR);

	tm t;
	memset(&t, 0, sizeof(tm));
	HibernateCalendarSet(&t);
	HibernateCalendarMatchSet(0, &t);
	HibernateIntClear(HIBERNATE_INT_PIN_WAKE | HIBERNATE_INT_LOW_BAT | HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntEnable(HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntRegister(&sntp::handlerTH);
	HibernateRTCEnable();
}

void sntp::task(void *pvParameters) {
	while(0x0 == lwIPLocalIPAddrGet() || 0xFFFFFFFF == lwIPLocalIPAddrGet()) {taskYIELD();}
	netconn* connection = NULL;
	s32 error = ERR_OK;
	while(true) {
		retryContext(connection, error);
		if(!connection) {
			UARTprintf("Out of memory, retry later\n");
		}
		else if(error != ERR_OK) {
			UARTprintf("Error occured: %d\n", error);
			netconn_close(connection);
			netconn_delete(connection);
			connection = NULL;
		}
		else {
			UARTprintf("Undefined error occured\n");
		}


		vTaskDelay(5000);
	}
}

bool sntp::processingReply(netbuf* reply) {
	u8 mode;
	sntp_msg* reference = NULL;
	if (reply->p->tot_len == 48) {
		netbuf_copy_partial(reply, &mode, sizeof(mode), reinterpret_cast<u32>(reference->li_vn_mode));
		mode &= SNTP_MODE_MASK;
		/* if this is a SNTP response... */
		if ((mode == SNTP_MODE_SERVER) || (mode == SNTP_MODE_BROADCAST)) {
			u8 stratum;
			netbuf_copy_partial(reply, &stratum, sizeof(stratum), reinterpret_cast<u32>(reference->stratum));
			if (stratum == SNTP_STRATUM_KOD) {
				// Kiss-of-death packet. Use another server or increase UPDATE_DELAY.
				UARTprintf("Kiss-of-death packet\n");
				return false;
			}
		}
	}

	u32 receive_timestamp = 0;
	netbuf_copy_partial(reply, &receive_timestamp, sizeof(receive_timestamp), reinterpret_cast<u32>(reference->receive_timestamp));

	netbuf_delete(reply);

	receive_timestamp = (ntohl(receive_timestamp) - DIFF_SEC_1900_1970);
	tm* t = std::localtime(&receive_timestamp);

	// The hour has to be incremented, because the Budapest
	//	time zone is shifted by one related to the GMT.
	t->tm_hour++;

	// The TivaC library add 100 to the year.
	// It's incomprehensible, why...
	t->tm_year += 100;
	HibernateCalendarSet(t);

	//Guard the message printing.
	taskENTER_CRITICAL();
	HibernateCalendarGet(t);
	UARTprintf("Date and time is set to: %d.%d.%d %d:%d:%d\n",
			t->tm_mday,
			t->tm_mon + 1, // increment the month due to human readable format
			t->tm_year + 1970 - 100, // add the date offset: 1970, and subtract the library "compensation"
			t->tm_hour,
			t->tm_min,
			t->tm_sec);
	taskEXIT_CRITICAL();

	return true;
}

netbuf* sntp::generateRequest() {
	static netbuf* request = NULL;
	if(!request) {
		request = netbuf_new();
		static sntp_msg msg;
		msg.li_vn_mode[0] = (SNTP_LI_NO_WARNING | SNTP_VERSION | SNTP_MODE_CLIENT);
		netbuf_ref(request, &msg, sizeof(msg));
	}

	return request;
}

void sntp::handlerTH() {
	HibernateIntClear(HIBERNATE_INT_RTC_MATCH_0);
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: sntp.cpp ============================
