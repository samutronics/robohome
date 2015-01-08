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

report			weather::_report;
ip_addr			weather::_serverIP;
tcp_pcb*		weather::_pcb;
weatherRequest	weather::_request;
bool			weather::_tcpRequestReceived;

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
	if(ERR_OK != netconn_gethostbyname(url, &_serverIP)) {
		UARTprintf("weather: something went wrong to get dns address\n");
		while(1);
	}

	UARTprintf("%s IP is: %d.%d.%d.%d\n",
			url,
			(_serverIP.addr & 0xff),
			((_serverIP.addr >> 8) & 0xff),
			((_serverIP.addr >> 26) & 0xff),
			((_serverIP.addr >> 24) & 0xff));

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





		netconn* connection = NULL;
		connection = netconn_new(NETCONN_TCP);
		if (connection == NULL) {UARTprintf("failed to create new connection\n"); while(1);}

//		local_ip.addr = <get IP of this device>

//		ip_addr addr;
//		addr.addr = lwIPLocalIPAddrGet();
//		netconn_bind(connection, &addr, 40000);

//		remote_ip.addr = xRemoteIp; // static or by netconn_gethostbyname ()
//		rc2 = ;

		if (ERR_OK != netconn_connect(connection, &_serverIP, port)) {UARTprintf("failed to connect server\n"); while(1);}

//		  netconn_delete ( xNetConn );


		UARTprintf("Successful connection to the server\n");

		UARTprintf("state: %d\n", connection->state);

		if (ERR_OK != netconn_write(connection, _request.request, _request.len, NETCONN_COPY)) {while(1);}
		UARTprintf("data is sent successful\n");

		UARTprintf("state: %d\n", connection->state);

		UARTprintf("local IP: %d\n", lwIPLocalIPAddrGet());
		UARTprintf("local IP: %d\n", connection->pcb.tcp->local_ip.addr);
		UARTprintf("local port: %d\n", connection->pcb.tcp->local_port);
		UARTprintf("remote IP: %d\n", connection->pcb.tcp->remote_ip.addr);
		UARTprintf("remote port: %d\n", connection->pcb.tcp->remote_port);

		netbuf* buf = NULL;
		while(1) {
			if (ERR_OK == netconn_recv(connection, &buf)) {

			UARTwrite((char*)buf->p->payload, buf->p->len);
			UARTprintf("===============================================================================================================================\n");
			UARTwrite((char*)buf->ptr->payload, buf->ptr->len);
			}
		}




		while(1);
























		while(ESTABLISHED != _pcb->state) {
		connectToServer();
		vTaskDelay(timeOut);
		if(ESTABLISHED != _pcb->state) {UARTprintf("Weather service: connect to server failed, try again!\n");}
		}

		// =============================================================================
		//! * Write the request to the tcp socket, and flush the data
		// =============================================================================
		_tcpRequestReceived = false;
		sendRequest();

		// =============================================================================
		//! * Close the socket, so that the LwIP resources are freed.
		// =============================================================================
		vTaskDelay(timeOut);
		closeConnection(_pcb);
		if(!_tcpRequestReceived) {UARTprintf("Response of weather request failed, try again!\n");}
		else {
		// =============================================================================
		//! * Wait until the time of the next request
		// =============================================================================
		vTaskDelay(updatePeriode);
		}
	}
}

void weather::closeConnection(tcp_pcb* psPcb) {
	if(psPcb) {
		//
		// Clear out all of the TCP callbacks.
		//
		tcp_sent(psPcb, 0);
		tcp_recv(psPcb, 0);
		tcp_err(psPcb, 0);

		//
		// Close the TCP connection.
		//
		tcp_close(psPcb);
        if(psPcb == _pcb) {
            _pcb = 0;
        }
	}
}

err_t weather::connectToServer() {
	closeConnection(_pcb);

	//
    // Create a new TCP socket.
    //
    _pcb = tcp_new();

    //
    // Attempt to connect to the server directly.
    //
    return tcp_connect(_pcb, &_serverIP, port, connectToServerCallback);
}

err_t weather::sendRequest() {
    err_t retVal = tcp_write(_pcb, _request.request, _request.len, TCP_WRITE_FLAG_COPY);

    //
    //  Write data for sending (but does not send it immediately).
    //
    if(retVal == ERR_OK) {
        //
        // Find out what we can send and send it
        //
        retVal = tcp_output(_pcb);
    }

    return retVal;
}

err_t weather::connectToServerCallback(void *pvArg, struct tcp_pcb *psPcb, err_t iErr) {
    //
    // Check if there was a TCP error.
    //
    if(iErr != ERR_OK) {
    	closeConnection(psPcb);
        return (ERR_OK);
    }

    tcp_recv(psPcb, TCPReceiveCallback);
    tcp_err(psPcb, TCPErrorCallback);
    tcp_sent(psPcb, TCPSentCallback);

    return(ERR_OK);
}

// =============================================================================
//! If an empty buffer arrived, the callbeck will close the socket, and notify
//! the service about the successful processing. In any other cases, checks the
//! request type stored locally and parse as JSON object.
//! In case of successful parsing, the important report items will be wrtitten
//! into the terminal.
// =============================================================================
err_t weather::TCPReceiveCallback(void* pvArg, struct tcp_pcb* psPcb, struct pbuf* psBuf, err_t iErr) {
	struct pbuf *psBufCur;
	int32_t i32Items;

	if(!psBuf) {
		closeConnection(psPcb);
		return(ERR_OK);
	}

	if(_request.type == currentRequest) {
		//
		// Read items from the buffer.
		//
		i32Items = JSONParseCurrent(0, _report, psBuf);

		//
		// Make sure some items were found.
		//
		if(i32Items > 0) {
			//Guard the message printing.
			taskENTER_CRITICAL();
			UARTprintf("Temperature: %d C\n",	_report.Temp);
			UARTprintf("Humidity: %d %%\n",		_report.Humidity);
			UARTprintf("Pressure: %d hpa\n",	_report.Pressure);
			taskEXIT_CRITICAL();
		}
		else if(i32Items < 0) {
			UARTprintf("Invalid request arrived\n");
		}
	}
	else if(_request.type == forecastRequest) {
		//
		// Read items from the buffer.
		//
		i32Items = JSONParseForecast(0, _report, psBuf);

		if(i32Items > 0) {
			//Guard the message printing.
			taskENTER_CRITICAL();
			UARTprintf("Temperature: %d C\n",	_report.Temp);
			UARTprintf("Humidity: %d %%\n",		_report.Humidity);
			UARTprintf("Pressure: %d hpa\n",	_report.Pressure);
			taskEXIT_CRITICAL();
		}
		else if(i32Items < 0) {
			UARTprintf("Invalid request arrived\n");
		}
	}

	_tcpRequestReceived = true;

	//
	// Initialize the linked list pointer to parse.
	//
	psBufCur = psBuf;

	//
	// Free the buffers used since they have been processed.
	//
	while(psBufCur->len != 0) {
		//
		// Indicate that you have received and processed this set of TCP data.
		//
		tcp_recved(psPcb, psBufCur->len);

		//
		// Go to the next buffer.
		//
		psBufCur = psBufCur->next;

		//
		// Terminate if there are no more buffers.
		//
		if(psBufCur == 0) {break;}
	}

	//
	// Free the memory space allocated for this receive.
	//
	pbuf_free(psBuf);
	return(ERR_OK);
}

err_t weather::TCPSentCallback(void* pvArg, struct tcp_pcb* psPcb, u16_t ui16Len) {return (ERR_OK);}
void weather::TCPErrorCallback(void*, err_t) {}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: weather.cpp =========================
