// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#include "lwIP/err.h"
#include "lwIP/ip_addr.h"
#include "lwIP/dns.h"
#include "exosite.hpp"
#include "requestFactory.hpp"
#include "exositemanager.hpp"
#include "devicestatistic.hpp"
#include "../projectconfiguration.hpp"

using namespace manager::exositeTask;
using namespace manager::exositeTask::configuration;

ip_addr											exositeManager::_serverIP;
tcp_pcb*										exositeManager::_pcb;
exositeManager::state							exositeManager::_state = idle;
basicVector<u8, exositeManager::_rxTxBufSize>	exositeManager::_rxTxBuf;

exositeManager::exositeManager() {
}

void exositeManager::task(void *pvParameters) {
	// check the server IP address at the first start and resolve the URL
    if(0 == _serverIP.addr || 0xFFFFFFFF == _serverIP.addr) {
    	// very stupid solution: wait until the LwIP stack is initialized
    	// it would be better, that the LwIP initialize can send some signal, like events, or semaphore.
        while(ERR_ARG == dns_gethostbyname(serverURL, &_serverIP, resolveHostCallback, 0)) {taskYIELD();}
    }

    // waiting for the server address
	while(0 == _serverIP.addr || 0xFFFFFFFF == _serverIP.addr) {taskYIELD();}

	u8 pucMACAddr[6];
	EMACAddrGet(EMAC0_BASE, 0, pucMACAddr);
	exosite::init("texasinstruments", "ek-tm4c1294xl", IF_ENET, pucMACAddr, 0);

	while(1) {
		requestFactory::makeDeviceSyncRequest();

		if(0 != requestFactory::writeRequestOutbound.len) {
			_state = writeRequestSent;
			exosite::write(requestFactory::writeRequestOutbound, _rxTxBuf);

	    	UARTprintf("\n================================Sent: =====================================\n");
			UARTwrite((s8*)_rxTxBuf.container, _rxTxBuf.len);
	    	UARTprintf("\n=====================================================================\n");

	    	// send a connection request to the server
	    	connectToServer();

	    	// wait for the proper connection state.
	    	// it seems to be a suitable solution
	    	while(ESTABLISHED != _pcb->state) {taskYIELD();}

			sendRequest();
			_rxTxBuf.len = 0;
			while(writeRequestProcessed != _state) {taskYIELD();}

			closeConnection();
		}

		if(0 != requestFactory::readRequestOutbound.len) {
			_state = readRequestSent;
			exosite::read(requestFactory::readRequestOutbound, _rxTxBuf);

	    	UARTprintf("\n================================Sent: =====================================\n");
			UARTwrite((s8*)_rxTxBuf.container, _rxTxBuf.len);
	    	UARTprintf("\n=====================================================================\n");

	    	// send a connection request to the server
	    	connectToServer();

	    	// wait for the proper connection state.
	    	// it seems to be a suitable solution
	    	while(ESTABLISHED != _pcb->state) {taskYIELD();}

			sendRequest();
			_rxTxBuf.len = 0;
			while(readRequestProcessed != _state) {taskYIELD();}

			closeConnection();
		}

		vTaskDelay(4000);
		//		taskYIELD();
	}
}

void exositeManager::closeConnection() {
	if(_pcb)
	{
		//
		// Clear out all of the TCP callbacks.
		//
		tcp_sent(_pcb, 0);
		tcp_recv(_pcb, 0);
		tcp_err(_pcb, 0);

		//
		// Close the TCP connection.
		//
		tcp_close(_pcb);
		_pcb = 0;
	}
}

err_t exositeManager::connectToServer() {
	if(_pcb)
	{
		//
        // Initially clear out all of the TCP callbacks.
        //
        tcp_sent(_pcb, 0);
        tcp_recv(_pcb, 0);
        tcp_err(_pcb, 0);

        //
        // Make sure there is no lingering TCP connection.
        //
        tcp_close(_pcb);
    }

    //
    // Create a new TCP socket.
    //
    _pcb = tcp_new();

    //
    // Attempt to connect to the server directly.
    //
    return tcp_connect(_pcb, &_serverIP, serverPort, connectToServerCallback);
}

err_t exositeManager::sendRequest() {
    err_t retVal = tcp_write(_pcb, _rxTxBuf.container, _rxTxBuf.len, TCP_WRITE_FLAG_COPY);

	//
    //  Write data for sending (but does not send it immediately).
    //
    if(retVal == ERR_OK)
    {
        //
        // Find out what we can send and send it
        //
        retVal = tcp_output(_pcb);
    }

    return retVal;
}

void exositeManager::resolveHostCallback(const char* pcName, struct ip_addr* psIPAddr, void* vpArg) {
	if(psIPAddr) {
		_serverIP = *psIPAddr;
	}
}

err_t exositeManager::connectToServerCallback(void *pvArg, struct tcp_pcb *psPcb, err_t iErr) {
    //
    // Check if there was a TCP error.
    //
    if(iErr != ERR_OK)
    {
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
        if(psPcb == _pcb)
        {
            _pcb = 0;
        }

        return (ERR_OK);
    }

    tcp_recv(psPcb, TCPReceiveCallback);
    tcp_err(psPcb, TCPErrorCallback);
    tcp_sent(psPcb, TCPSentCallback);

    //
    // Return a success code.
    //
    return(ERR_OK);
}

err_t exositeManager::TCPReceiveCallback(void* pvArg, struct tcp_pcb* psPcb, struct pbuf* psBuf, err_t iErr) {
	struct pbuf *psBufCur;
	int32_t i32Items;

	if(!psBuf)
	{
		//
		// Close out the port.
		//
		tcp_close(psPcb);

		if(psPcb == _pcb)
		{
			_pcb = 0;
		}

		return(ERR_OK);
	}


	UARTprintf("\n================================Received: =====================================\n");
	UARTwrite((s8*)psBuf->payload, psBuf->tot_len);
	UARTprintf("\n=====================================================================\n");

	if(writeRequestSent == _state) {
		_state = writeRequestProcessed;
	}
	else if(readRequestSent == _state) {
		_state = readRequestProcessed;
	}

/*
	if(_request.type == currentRequest)
	{
		//
		// Read items from the buffer.
		//
		i32Items = JSONParseCurrent(0, _report, psBuf);

		//
		// Make sure some items were found.
		//
		if(i32Items > 0)
		{
			UARTprintf("Temperature: %i\n",	_report.Temp);
			UARTprintf("Humidity: %i\n",	_report.Humidity);
			UARTprintf("Pressure: %i\n",	_report.Pressure);
		}
		else if(i32Items < 0)
		{
			UARTprintf("Invalid request arrived");
		}
	}
	else if(_request.type == forecastRequest)
	{
		//
		// Read items from the buffer.
		//
		i32Items = JSONParseForecast(0, _report, psBuf);

		if(i32Items > 0)
		{
			UARTprintf("Temperature: %i\n",	_report.Temp);
			UARTprintf("Humidity: %i\n",	_report.Humidity);
			UARTprintf("Pressure: %i\n",	_report.Pressure);
		}
		else if(i32Items < 0)
		{
			UARTprintf("Invalid request arrived");
		}
	}
*/
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

	//
	// Return.
	//
	return(ERR_OK);
}

err_t exositeManager::TCPSentCallback(void* pvArg, struct tcp_pcb* psPcb, u16_t ui16Len) {return (ERR_OK);}
void exositeManager::TCPErrorCallback(void*, err_t) {
}

void exositeManager::socketClose(long lSocket) {
}

u8 exositeManager::socketSend(long lSocket, s8* buffer, s32 length) {
}

u8 exositeManager::socketRecv(long lSocket, s8* buffer, s32 length) {
}
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: exositemanager.cpp =======================
