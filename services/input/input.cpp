// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			04.12.2014.
//! \note
// =============================================================================
#include "input.hpp"
#include "ipcQueue.hpp"
#include "../projectconfiguration.hpp"

using namespace communication::ipc;
using namespace service::inboundTask;
using namespace service::inboundTask::configuration;

DECLARE_TH(input)

input::input() {
	_THQueue = xQueueCreate(THQueueLength, THQueueWidth);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
    GPIOIntRegister(GPIO_PORTJ_BASE, &input::handlerTH);
    GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
}

void input::task(void *pvParameters) {
	while(1) {
		// The thread gives up its time-slice, if the TH queue is empty: there was no interrupt
		while(0 == uxQueueMessagesWaiting(_THQueue)) {taskYIELD();}

		// If item received, read it from the TH queue
		u8 state;
		xQueueReceive(_THQueue, &state, 0);

		// This is the place, where the thread couly process the datas

		// Forward the item to the targeted thread
		xQueueSendToBack(ipcQueue::singleton().queue(outboundQueue), &state, NULL);

		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void input::handlerTH() {
	static bool loggedState = false;
	loggedState = !loggedState;
	xQueueSendToBackFromISR(_THQueue, &loggedState, NULL);
	GPIOIntClear(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
}

// =============================================================================
//! \file
//! \copyright
// ======================== end of file: input.cpp =============================
