// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			04.12.2014.
//! \note
// =============================================================================
#include "input.hpp"
#include "../projectconfiguration.hpp"

using namespace service::inbound;
using namespace service::inbound::configuration;

DECLARE_TH(input)

input::input() {
	_THQueue = xSemaphoreCreateBinary();

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure	(timer, TIMER_CFG_PERIODIC);
	TimerLoadSet	(timer, TIMER_A, systemGlobal::requestedSystemClockFrequency / pollingFrequency);
	TimerIntRegister(timer, TIMER_A, &handlerTH);
	TimerIntEnable	(timer, TIMER_TIMA_TIMEOUT);
	TimerEnable		(timer, TIMER_A);
}

void input::task(void *pvParameters) {
	while(true) {
		// The thread gives up its time-slice, if the TH queue is empty: there was no interrupt
		xSemaphoreTake(_THQueue, portMAX_DELAY);


		UARTprintf("C\n");


		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void input::handlerTH() {
	TimerIntClear(timer, TIMER_TIMA_TIMEOUT);
	xSemaphoreGiveFromISR(_THQueue, NULL);
}

// =============================================================================
//! \file
//! \copyright
// ======================== end of file: input.cpp =============================
