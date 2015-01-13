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

using namespace service::weather;
using namespace service::weather::configuration;

weather::weather() {
}

void weather::task(void *pvParameters) {
	while(0x0 == lwIPLocalIPAddrGet() || 0xFFFFFFFF == lwIPLocalIPAddrGet()) {taskYIELD();}
	// =============================================================================
	//! * At this point, the excecution has to wait for the end of the
	//!		inicialization of the dns module. Until that, argument error will be
	//!		returned from the dns lookup.
	//!	\note Refactoring of each of LwIP services would be nice to became queriable
	//! about its state, or the service send any kind of event.
	// =============================================================================
	if(ERR_OK != netconn_gethostbyname(url, &_serverIP)) {UARTprintf("weather: something went wrong to get dns address\n"); while(1);}

	// =============================================================================
	//! * Create HTTP get request to query the actual weather informations. It has
	//! to be done only once.
	//! \warning Whitespaces are not allowed in the the request.
	// =============================================================================
	weatherRequestFactory::request(_request, "Budapest,HU", false, 0);

	while(1) {
		// =============================================================================
		//! .
		//! The further steps are excecuted periodically.
		//! * Connect to the server, and block the execution until the connection will
		//! be established.
		// =============================================================================
		netconn* connection = netconn_new(NETCONN_TCP);
		if (connection == NULL) {UARTprintf("failed to create new connection\n"); while(1);}

		if (ERR_OK != netconn_connect(connection, &_serverIP, port)) {UARTprintf("failed to connect server\n"); while(1);}

		if (ERR_OK != netconn_write(connection, _request.request, _request.len, NETCONN_COPY)) {UARTprintf("failed to write into connection\n"); while(1);}

		netbuf* buf = NULL;
		if (ERR_OK != netconn_recv(connection, &buf)) {UARTprintf("failed to receive from connection\n"); while(1);}

		u32 itemCount = JSONParseCurrent(0, _report, buf->p);

		netbuf_delete(buf);

		if(0 < itemCount) {
			//Guard the message printing.
			taskENTER_CRITICAL();
			UARTprintf("Temperature: %d C\n",	_report.Temp);
			UARTprintf("Humidity: %d %%\n",		_report.Humidity);
			UARTprintf("Pressure: %d hpa\n",	_report.Pressure);
			taskEXIT_CRITICAL();
		}
		else {
			UARTprintf("Failed to parse request");
		}

		netconn_close(connection);
		netconn_delete(connection);

		// =============================================================================
		//! * Wait until the time of the next request
		// =============================================================================
		vTaskDelay(updatePeriode);
	}
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: weather.cpp =========================
