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

weather::weather() {
}

void weather::task(void *pvParameters) {
	while(0x0 == lwIPLocalIPAddrGet() || 0xFFFFFFFF == lwIPLocalIPAddrGet()) {taskYIELD();}
	netconn* connection = NULL;
	s32 error = ERR_OK;
	while(true) {
		retryContext(connection, error);
		if(!connection) {
			UARTprintf("Out of memory, retry later\n");
		}

		if(error != ERR_OK) {
			UARTprintf("Error occured: %d\n", error);
			netconn_close(connection);
			netconn_delete(connection);
			connection = NULL;
		}

		vTaskDelay(5000);
	}
}

void weather::retryContext(netconn*& connection, s32& error) {
	// =============================================================================
	//! * At this point, the excecution has to wait for the end of the
	//!		inicialization of the dns module. Until that, argument error will be
	//!		returned from the dns lookup.
	//!	\note Refactoring of each of LwIP services would be nice to became queriable
	//! about its state, or the service send any kind of event.
	// =============================================================================
	error = netconn_gethostbyname(url, &_serverIP);
	if(ERR_OK != error) {return;}

	// =============================================================================
	//! * Create HTTP get request to query the actual weather informations. It has
	//! to be done only once.
	//! \warning Whitespaces are not allowed in the the request.
	// =============================================================================
	const string& request = _requestFactory.request("Budapest,HU", false, 0);

	while(1) {
		// =============================================================================
		//! .
		//! The further steps are excecuted periodically.
		//! * Connect to the server, and block the execution until the connection will
		//! be established.
		// =============================================================================
		connection = netconn_new(NETCONN_TCP);
		if (connection == NULL) {return;}

		error = netconn_connect(connection, &_serverIP, port);
		if (ERR_OK != error) {return;}

		error = netconn_write(connection, request.data(), request.length(), NETCONN_NOCOPY);
		if (ERR_OK != error) {return;}

		netbuf* buf = NULL;
		error = netconn_recv(connection, &buf);
		if (ERR_OK != error) {netbuf_delete(buf); return;}

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
			UARTprintf("Failed to parse request\n");
		}

		netconn_close(connection);
		netconn_delete(connection);

		if(0 == itemCount) {
			connection = NULL;
			return;
		}

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
