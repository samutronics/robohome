// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#include "lwip/api.h"
#include "exosite.hpp"
#include "requestFactory.hpp"
#include "devicestatistic.hpp"
#include "exositerequestfactory.hpp"
#include "../projectconfiguration.hpp"

using namespace service::exosite;
using namespace service::exosite::configuration;

exosite::exosite() {
}

void exosite::task(void *pvParameters) {
	while(0x0 == lwIPLocalIPAddrGet() || 0xFFFFFFFF == lwIPLocalIPAddrGet()) {taskYIELD();}
	// very stupid solution: wait until the LwIP stack is initialized
	// it would be better, that the LwIP initialize can send some signal, like events, or semaphore.

	u8 pucMACAddr[6];
	EMACAddrGet(EMAC0_BASE, 0, pucMACAddr);
	exositeRequestFactory::init("texasinstruments", "ek-tm4c1294xl", IF_ENET, pucMACAddr, 0);

	if(ERR_OK != netconn_gethostbyname(url, &_serverIP)) {UARTprintf("exosite: something went wrong to get dns address\n"); while(1);}

	netconn* connection = netconn_new(NETCONN_TCP);
	if (connection == NULL) {UARTprintf("failed to create new connection\n"); while(1);}

//	if (ERR_OK != netconn_connect(connection, &_serverIP, port)) {UARTprintf("failed to connect server\n"); while(1);}
	while(true) {
		if (ERR_OK == netconn_connect(connection, &_serverIP, port)) {break;}
			UARTprintf("failed to connect server\n");
			vTaskDelay(1500);
//			while(1);
	}

	while(1) {
		deviceRequestFactory::makeDeviceSyncRequest();

		if(0 != deviceRequestFactory::writeRequestOutbound.len) {
			exositeRequestFactory::write(deviceRequestFactory::writeRequestOutbound, _rxTxBuf);
			if (ERR_OK != netconn_write(connection, _rxTxBuf.container, _rxTxBuf.len, NETCONN_COPY)) {UARTprintf("failed to write into connection\n"); while(1);}
			_rxTxBuf.len = 0;

			netbuf* buf = NULL;
			if (ERR_OK != netconn_recv(connection, &buf)) {UARTprintf("failed to receive from connection\n"); while(1);}
			exositeRequestFactory::parseWriteResult(buf->p);
			netbuf_delete(buf);
		}

		vTaskDelay(updatePeriode);

		if(0 != deviceRequestFactory::readRequestOutbound.len) {
			exositeRequestFactory::read(deviceRequestFactory::readRequestOutbound, _rxTxBuf);
			if (ERR_OK != netconn_write(connection, _rxTxBuf.container, _rxTxBuf.len, NETCONN_COPY)) {UARTprintf("failed to write into connection\n"); while(1);}
			_rxTxBuf.len = 0;
			netbuf* buf = NULL;
			if (ERR_OK != netconn_recv(connection, &buf)) {UARTprintf("failed to receive from connection\n"); while(1);}

			UARTprintf("\n");
			UARTwrite((char*)buf->p->payload, buf->p->len);
			UARTprintf("\n");

			exositeRequestFactory::parseReadResult(buf->p, deviceRequestFactory::response);
			netbuf_delete(buf);

//			while(readRequestProcessed != _state) {taskYIELD();}

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
		}

		vTaskDelay(updatePeriode);
	}
}

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.cpp ==========================
