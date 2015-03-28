// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.27.2015.
//! \note
// =============================================================================
#include "irrigation.hpp"
#include "../projectconfiguration.hpp"

using namespace service::irrigation;
using namespace service::irrigation::configuration;

DECLARE_TH(irrigation)

irrigation::irrigation() {
	_THQueue = xSemaphoreCreateBinary();
	timerStart();
}

void irrigation::task(void *pvParameters) {
	while(true) {
		// The thread gives up its time-slice, if there is no semaphire given.
		xSemaphoreTake(_THQueue, portMAX_DELAY);

		UARTprintf("irrigation\n");

		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void irrigation::timerStart() const {
	SysCtlPeripheralEnable(timerPeriphery);
	TimerConfigure	(timer, TIMER_CFG_PERIODIC);
	TimerLoadSet	(timer, TIMER_A, systemGlobal::requestedSystemClockFrequency / pollingFrequency);
	TimerIntRegister(timer, TIMER_A, &handlerTH);
	TimerIntEnable	(timer, TIMER_TIMA_TIMEOUT);
	TimerEnable		(timer, TIMER_A);
}

void irrigation::handlerTH() {
	TimerIntClear(timer, TIMER_TIMA_TIMEOUT);
	xSemaphoreGiveFromISR(_THQueue, NULL);
}

// =============================================================================
//! \file
//! \copyright
// ======================= end of file: irrigation.cpp =========================
