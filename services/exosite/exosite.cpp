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

	netconn* connection = NULL;
	s32 error = ERR_OK;
	while(true) {
		retryContext(connection, error);
		if(!connection) {
			UARTprintf("Out of memory, retry later\n");
		}

		if(error != ERR_OK) {
			UARTprintf("Error occured: %d\n", error);
		}

		vTaskDelay(5000);
	}
}

void exosite::retryContext(netconn*& connection, s32& error) {
	error = netconn_gethostbyname(url, &_serverIP);
	if(ERR_OK != error) {return;}

	connection = netconn_new(NETCONN_TCP);
	if (connection == NULL) {return;}

	error = netconn_connect(connection, &_serverIP, port);
	if (ERR_OK != error) {return;}

	while(1) {
		deviceRequestFactory::makeDeviceSyncRequest();
		if(0 != deviceRequestFactory::writeRequestOutbound.len) {
			exositeRequestFactory::write(deviceRequestFactory::writeRequestOutbound, _rxTxBuf);
			error = netconn_write(connection, _rxTxBuf.container, _rxTxBuf.len, NETCONN_COPY);
			if (ERR_OK != error) {return;}

			_rxTxBuf.len = 0;
			netbuf* buf = NULL;
			error = netconn_recv(connection, &buf);
			if (ERR_OK != error) {netbuf_delete(buf); return;}
			exositeRequestFactory::parseWriteResult(buf->p);
			netbuf_delete(buf);
		}

		vTaskDelay(updatePeriode);

		if(0 != deviceRequestFactory::readRequestOutbound.len) {
			exositeRequestFactory::read(deviceRequestFactory::readRequestOutbound, _rxTxBuf);
			error = netconn_write(connection, _rxTxBuf.container, _rxTxBuf.len, NETCONN_COPY);
			if (ERR_OK != error) {return;}

			_rxTxBuf.len = 0;
			netbuf* buf = NULL;
			error = netconn_recv(connection, &buf);
			if (ERR_OK != error) {netbuf_delete(buf); return;}
			exositeRequestFactory::parseReadResult(buf->p, deviceRequestFactory::response);
			netbuf_delete(buf);

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
