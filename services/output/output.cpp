// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \note
// =============================================================================
#include "output.hpp"
#include "ipcQueue.hpp"
#include "../projectconfiguration.hpp"

using namespace communication::ipc;
using namespace manager::outboundTask;
using namespace manager::outboundTask::configuration;

outputManager::outputManager() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIODirModeSet(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT);

    _queueOutbound = xQueueCreate(outboundQueueLength, outboundQueueWidth);
}

void outputManager::task(void *pvParameters) {
	while(1) {
		// Query the queue handler due to performance reason.
		xQueueHandle queueHandle = ipcQueue::singleton().queue(outboundQueue);

		// The thread gives up its time-slice, if the TH queue is empty: there was no interrupt
		while(0 == uxQueueMessagesWaiting(queueHandle)) {taskYIELD();}

		// If item received, read it from the TH queue
		u8 state;
		xQueueReceive(queueHandle, &state, 0);

		// Forward the data to the target: in this particular case to the LED
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, (state << 1));

		// The task gives up its remained time-slice
		taskYIELD();
	}
}

// =============================================================================
//! \file
//! \copyright
// ===================== end of file: outputmanager.cpp ========================
