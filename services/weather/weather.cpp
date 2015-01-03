// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#include "json.hpp"
#include "lwIP/dns.h"
#include "lwIP/tcp.h"
#include "lwIP/pbuf.h"
#include "weather.hpp"
#include "requestfactory.hpp"

using namespace service::weatherTask;
using namespace service::weatherTask::configuration;

report			weather::_report;
ip_addr			weather::_serverIP;
tcp_pcb*		weather::_pcb;
weatherRequest	weather::_request;

weather::weather() {
}

void weather::task(void *pvParameters) {
	// check the server IP address at the first start and resolve the URL
    if(0 == _serverIP.addr || 0xFFFFFFFF == _serverIP.addr) {
    	// very stupid solution: wait until the LwIP stack is initialized
    	// it would be better, that the LwIP initialize can send some signal, like events, or semaphore.
        while(ERR_ARG == dns_gethostbyname(weatherServerURL, &_serverIP, resolveHostCallback, 0)) {taskYIELD();}
    }

    // waiting for the server address
	while(0 == _serverIP.addr || 0xFFFFFFFF == _serverIP.addr) {taskYIELD();}

	// create the request.
	// Please note, that whitespaces aren't allowed in the request
	weatherRequestFactory::request(_request, "Budapest,HU", false, 0);

	while(1) {
		// send a connection request to the server
		connectToServer();

		// wait for the proper connection state.
		// it seems to be a suitable solution
		while(ESTABLISHED != _pcb->state) {taskYIELD();}

		// write the request into the TCP socket.
		sendRequest();

		// block the task until the update time elapses.
		vTaskDelay(weatherReportUpdateTime);
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
    return tcp_connect(_pcb, &_serverIP, weatherServerPort, connectToServerCallback);
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

void weather::resolveHostCallback(const char* pcName, struct ip_addr* psIPAddr, void* vpArg) {
	if(psIPAddr) {
		_serverIP = *psIPAddr;
	}
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
			UARTprintf("Invalid request arrived");
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
			UARTprintf("Invalid request arrived");
		}
	}

	//
	// Initialize the linked list pointer to parse.
	//
	psBufCur = psBuf;

	//
	// Free the buffers used since they have been processed.
	//
	while(psBufCur->len != 0)
	{
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
		if(psBufCur == 0)
		{
			break;
		}
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
// ===================== end of file: weathermanager.cpp =======================
