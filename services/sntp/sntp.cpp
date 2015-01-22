// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			15.12.2014.
//! \note
// =============================================================================
#include "time.h"
#include "sntpd.h"
#include "sntp.hpp"
#include "lwip/api.h"
#include "ipcQueue.hpp"


PACK_STRUCT_BEGIN
struct sntp_msg {
 PACK_STRUCT_FIELD(u8_t           li_vn_mode);
 PACK_STRUCT_FIELD(u8_t           stratum);
 PACK_STRUCT_FIELD(u8_t           poll);
 PACK_STRUCT_FIELD(u8_t           precision);
 PACK_STRUCT_FIELD(u32_t          root_delay);
 PACK_STRUCT_FIELD(u32_t          root_dispersion);
 PACK_STRUCT_FIELD(u32_t          reference_identifier);
 PACK_STRUCT_FIELD(u32_t          reference_timestamp[2]);
 PACK_STRUCT_FIELD(u32_t          originate_timestamp[2]);
 PACK_STRUCT_FIELD(u32_t          receive_timestamp[2]);
 PACK_STRUCT_FIELD(u32_t          transmit_timestamp[2]);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

#define SNTP_OFFSET_LI_VN_MODE      0
#define SNTP_LI_MASK                0xC0
#define SNTP_LI_NO_WARNING          0x00
#define SNTP_LI_LAST_MINUTE_61_SEC  0x01
#define SNTP_LI_LAST_MINUTE_59_SEC  0x02
#define SNTP_LI_ALARM_CONDITION     0x03 /* (clock not synchronized) */

#define SNTP_VERSION_MASK           0x38
#define SNTP_VERSION                (4/* NTP Version 4*/<<3)

#define SNTP_MODE_MASK              0x07
#define SNTP_MODE_CLIENT            0x03
#define SNTP_MODE_SERVER            0x04
#define SNTP_MODE_BROADCAST         0x05

#define SNTP_OFFSET_STRATUM         1
#define SNTP_STRATUM_KOD            0x00

#define SNTP_OFFSET_ORIGINATE_TIME  24
#define SNTP_OFFSET_RECEIVE_TIME    32
#define SNTP_OFFSET_TRANSMIT_TIME   40

#define DIFF_SEC_1900_1970         (2208988800UL)

using namespace communication::ipc;
using namespace service::sntp;

sntp::sntp() {
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

	s32 error = netconn_gethostbyname("pool.ntp.org", &_serverIP);
	if(ERR_OK != error) {UARTprintf("error, stop sntp\n"); while(true);}

	netconn* connection = netconn_new(NETCONN_UDP);
	if (connection == NULL) {UARTprintf("error, stop sntp\n"); while(true);}

	error = netconn_connect(connection, &_serverIP, 123);
	if (ERR_OK != error) {return;}

	netbuf* buffer = netbuf_new();
	void* message = netbuf_alloc(buffer, 48);

	static_cast<sntp_msg*>(message)->li_vn_mode = (SNTP_LI_NO_WARNING | SNTP_VERSION | SNTP_MODE_CLIENT);

	error = netconn_send(connection, buffer);
	if (ERR_OK != error) {UARTprintf("error, stop sntp\n"); while(true);}

	netbuf_delete(buffer);
	buffer = NULL;
	error = netconn_recv(connection, &buffer);
	if (ERR_OK != error) {UARTprintf("error, stop sntp\n"); while(true);}
	UARTprintf("total length: %d\n", buffer->p->tot_len);
	UARTwrite((s8*)buffer->p->payload, buffer->p->len);

	u32 receive_timestamp = 0;
	pbuf_copy_partial(buffer->p, &receive_timestamp, 4, SNTP_OFFSET_RECEIVE_TIME);

	UARTprintf("received timestamp: %d\n", receive_timestamp);
	receive_timestamp = (ntohl(receive_timestamp) - DIFF_SEC_1900_1970);
	tm* t = std::localtime(&receive_timestamp);


	UARTprintf("Date and time is set to: %d.%d.%d %d:%d:%d\n",
			t->tm_mday,
			t->tm_mon + 1, // increment the month due to human readable format
			t->tm_year + 1970, // add the date offset: 1970, and subtract the library "compensation"
			t->tm_hour,
			t->tm_min,
			t->tm_sec);








	while(true);

	while(1) {
		// Query the queue handler due to performance reason.
		xQueueHandle queueHandle = ipcQueue::singleton().queue(rtcQueue);

		// The thread gives up its time-slice, if the TH queue is empty: there was no interrupt
		while(0 == uxQueueMessagesWaiting(queueHandle)) {taskYIELD();}

		// If item received, read it from the TH queue
		u32 time;
		xQueueReceive(queueHandle, &time, 0);

		tm* t = std::localtime(&time);

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

		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void sntp::handlerTH() {
	HibernateIntClear(HIBERNATE_INT_RTC_MATCH_0);
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: sntp.cpp ============================
