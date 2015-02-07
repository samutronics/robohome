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

weather::weather(): abstractclientservice(url, port, NETCONN_TCP, updatePeriode) {
}

bool weather::processingReply(netbuf* reply) {
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
	netbuf* buf = buf = netbuf_new();
	const string& request = _requestFactory.request("Budapest,HU", false, 0);
	netbuf_ref(buf, request.data(), request.length());

	return buf;
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: weather.cpp =========================
