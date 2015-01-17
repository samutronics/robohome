// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#include "lwip/api.h"
#include "exosite.hpp"
#include "devicestatistic.hpp"
#include "exositerequestfactory.hpp"
#include "../projectconfiguration.hpp"

using namespace std;
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
			netconn_close(connection);
			netconn_delete(connection);
			connection = NULL;
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
		exositeRequestFactory::write(deviceRequestFactory::writeRequest(), _rxTxBuf);
		error = netconn_write(connection, _rxTxBuf.data(), _rxTxBuf.length(), NETCONN_COPY);
		if (ERR_OK != error) {return;}

		netbuf* writeReplyBuffer = NULL;
		error = netconn_recv(connection, &writeReplyBuffer);
		if (ERR_OK != error) {netbuf_delete(writeReplyBuffer); return;}
		exositeRequestFactory::parseWriteResult(writeReplyBuffer->p);
		netbuf_delete(writeReplyBuffer);

		vTaskDelay(updatePeriode);

		exositeRequestFactory::read(deviceRequestFactory::readRequest(), _rxTxBuf);
		error = netconn_write(connection, _rxTxBuf.data(), _rxTxBuf.length(), NETCONN_COPY);
		if (ERR_OK != error) {return;}

		netbuf* readReplyBuffer = NULL;
		error = netconn_recv(connection, &readReplyBuffer);
		if (ERR_OK != error) {netbuf_delete(readReplyBuffer); return;}
		exositeRequestFactory::parseReadResult(readReplyBuffer->p, deviceRequestFactory::response);
		netbuf_delete(readReplyBuffer);

		deviceStatistic::reset();
		while(deviceStatistic::next()) {
			taskENTER_CRITICAL();
			deviceRequestFactory::updateEntryByResponse(*deviceStatistic::current());

			//Guard the message printing.
			taskENTER_CRITICAL();
			if(deviceStatistic::current()->entryName) {
				UARTprintf("%s=%s\n", deviceStatistic::current()->entryName, deviceStatistic::current()->getValue().c_str());
			}
			taskEXIT_CRITICAL(); //this is only for the systematic usage of print guarding

			taskEXIT_CRITICAL();
		}

		vTaskDelay(updatePeriode);
	}
}

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.cpp ==========================
