//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \details
//! \note
//! =============================================================================
#include "outputManager.hpp"

using namespace manager::task;
using namespace manager::configuration::outboundTask;

outputManager::outputManager() {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    ROM_GPIODirModeSet(GPIO_PORTN_BASE, GPIO_PIN_1,GPIO_DIR_MODE_OUT);

    _queueOutbound = xQueueCreate(outboundQueueLength, outboundQueueWidth);
}

void outputManager::task(void *pvParameters) {
	while(1) {
//	    ROM_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 2);

	    taskYIELD();
	}
}

//! =============================================================================
//! \copyright
//! ===================== end of file: outputmanager.cpp ========================
