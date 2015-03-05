// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \note
// =============================================================================
#include "output.hpp"
#include "../projectconfiguration.hpp"

using namespace service::outbound;
using namespace service::outbound::configuration;

DECLARE_TH(output)

output::output() {
	_THQueue = xSemaphoreCreateBinary();

	SysCtlPeripheralEnable(timerPeriphery);
	TimerConfigure	(timer, TIMER_CFG_PERIODIC);
	TimerLoadSet	(timer, TIMER_A, systemGlobal::requestedSystemClockFrequency / pollingFrequency);
	TimerIntRegister(timer, TIMER_A, &handlerTH);
	TimerIntEnable	(timer, TIMER_TIMA_TIMEOUT);
	TimerEnable		(timer, TIMER_A);
}

void output::task(void *pvParameters) {
	while(1) {
		// The thread gives up its time-slice, if there is no semaphire given.
		xSemaphoreTake(_THQueue, portMAX_DELAY);


		UARTprintf("D\n");


		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void output::handlerTH() {
	TimerIntClear(timer, TIMER_TIMA_TIMEOUT);
	xSemaphoreGiveFromISR(_THQueue, NULL);
}

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: output.cpp ===========================
