// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#include "json.hpp"
#include "lwip/api.h"
#include "weather.hpp"
#include "requestfactory.hpp"

using namespace std;
using namespace service::weather;
using namespace service::weather::configuration;

weather::weather(): asbtractServiceRequestTransmitter(url, port, NETCONN_TCP, updatePeriode) {
}

void weather::task(void *pvParameters) {
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

bool service::weather::weather::processingReply(netbuf* reply) {
	report r;
	u32 itemCount = JSONParseCurrent(0, r, reply->p);
	netbuf_delete(reply);
	if(0 < itemCount) {
		//Guard the message printing.
		taskENTER_CRITICAL();
		UARTprintf("Temperature: %d C\n",	r.Temp);
		UARTprintf("Humidity: %d %%\n",		r.Humidity);
		UARTprintf("Pressure: %d hpa\n",	r.Pressure);
		taskEXIT_CRITICAL();
	}
	else {
		UARTprintf("Failed to parse request\n");
	}

	return static_cast<bool>(itemCount);
}

netbuf* weather::generateRequest() {
	static netbuf* buf = NULL;
	if(!buf) {
		buf = netbuf_new();
		const string& request = _requestFactory.request("Budapest,HU", false, 0);
		netbuf_ref(buf, request.data(), request.length());
	}

	return buf;
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: weather.cpp =========================
