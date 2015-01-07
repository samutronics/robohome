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
#include "devicestatistic.hpp"
#include "exositerequestfactory.hpp"
#include "../projectconfiguration.hpp"

using namespace service::exositeTask;
using namespace service::exositeTask::configuration;

ip_addr									exosite::_serverIP;
tcp_pcb*								exosite::_pcb;
exosite::state							exosite::_state = idle;
basicVector<u8, exosite::_rxTxBufSize>	exosite::_rxTxBuf;

exosite::exosite() {
}

void exosite::task(void *pvParameters) {
	while(0x0 == lwIPLocalIPAddrGet() || 0xFFFFFFFF == lwIPLocalIPAddrGet()) {taskYIELD();}
	// very stupid solution: wait until the LwIP stack is initialized
	// it would be better, that the LwIP initialize can send some signal, like events, or semaphore.

	if(ERR_OK != netconn_gethostbyname(serverURL, &_serverIP)) {
		UARTprintf("exosite: something went wrong to get dns address\n");
		while(1);
	}

	UARTprintf("%s IP is: %d.%d.%d.%d\n",
			serverURL,
			(_serverIP.addr & 0xff),
			((_serverIP.addr >> 8) & 0xff),
			((_serverIP.addr >> 26) & 0xff),
			((_serverIP.addr >> 24) & 0xff));


//	while(ERR_ARG == dns_gethostbyname(serverURL, &_serverIP, resolveHostCallback, 0)) {taskYIELD();}

    // waiting for the server address
	while(0 == _serverIP.addr || 0xFFFFFFFF == _serverIP.addr) {taskYIELD();}

	u8 pucMACAddr[6];
	EMACAddrGet(EMAC0_BASE, 0, pucMACAddr);
	exositeRequestFactory::init("texasinstruments", "ek-tm4c1294xl", IF_ENET, pucMACAddr, 0);

	while(ESTABLISHED != _pcb->state) {
	connectToServer();
	vTaskDelay(connectionTimeOut);
	if(ESTABLISHED != _pcb->state) {UARTprintf("Exosite service: connect to server failed, try again!\n");}
	}

	while(1) {
		deviceRequestFactory::makeDeviceSyncRequest();

		if((0 != deviceRequestFactory::writeRequestOutbound.len) && (_state == idle)) {
			exositeRequestFactory::write(deviceRequestFactory::writeRequestOutbound, _rxTxBuf);
			sendRequest();
			_state = writeRequestSent;
			_rxTxBuf.len = 0;
			while(writeRequestProcessed != _state) {taskYIELD();}
			_state = idle;
		}

		vTaskDelay(updatePeriode / 2);

		if((0 != deviceRequestFactory::readRequestOutbound.len) && (_state == idle)) {
			exositeRequestFactory::read(deviceRequestFactory::readRequestOutbound, _rxTxBuf);
			sendRequest();
			_state = readRequestSent;
			_rxTxBuf.len = 0;
			while(readRequestProcessed != _state) {taskYIELD();}

		    deviceStatistic::reset();
		    while(deviceStatistic::next()) {
				taskENTER_CRITICAL();
		    	deviceRequestFactory::updateEntryByResponse(*deviceStatistic::current());

				//Guard the message printing.
				taskENTER_CRITICAL();
		    	if(deviceStatistic::current()->entryName) {
		    		static s8 value[statisticEntry::dataStringLength];
		    		deviceStatistic::current()->getValue(value);
		    		UARTprintf("%s=%s\n", deviceStatistic::current()->entryName, value);
		    	}
		    	taskEXIT_CRITICAL(); //this is only for the systematic usage of print guarding

		    	taskEXIT_CRITICAL();
		    }

		    _state = idle;
		}

		vTaskDelay(updatePeriode / 2);
	}
}

void exosite::closeConnection(tcp_pcb* psPcb) {
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

err_t exosite::connectToServer() {
	closeConnection(_pcb);

	//
	// Create a new TCP socket.
	//
	_pcb = tcp_new();

    //
    // Attempt to connect to the server directly.
    //
    return tcp_connect(_pcb, &_serverIP, serverPort, connectToServerCallback);
}

err_t exosite::sendRequest() {
    err_t retVal = tcp_write(_pcb, _rxTxBuf.container, _rxTxBuf.len, TCP_WRITE_FLAG_COPY);

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

void exosite::resolveHostCallback(const char* pcName, struct ip_addr* psIPAddr, void* vpArg) {
	if(psIPAddr) {
		_serverIP = *psIPAddr;
	}
}

err_t exosite::connectToServerCallback(void *pvArg, struct tcp_pcb *psPcb, err_t iErr) {
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

err_t exosite::TCPReceiveCallback(void* pvArg, struct tcp_pcb* psPcb, struct pbuf* psBuf, err_t iErr) {
	struct pbuf *psBufCur;
	if(!psBuf) {
		closeConnection(psPcb);
		return(ERR_OK);
	}

	if(writeRequestSent == _state) {
		_state = writeRequestProcessed;
		exositeRequestFactory::parseWriteResult(psBuf);
	}
	else if(readRequestSent == _state) {
		_state = readRequestProcessed;
		exositeRequestFactory::parseReadResult(psBuf, deviceRequestFactory::response);
	}

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
		if(!psBufCur) {break;}
	}

	//
	// Free the memory space allocated for this receive.
	//
	pbuf_free(psBuf);
	return(ERR_OK);
}

err_t exosite::TCPSentCallback(void* pvArg, struct tcp_pcb* psPcb, u16_t ui16Len) {return (ERR_OK);}
void exosite::TCPErrorCallback(void*, err_t) {
}

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.cpp ==========================
