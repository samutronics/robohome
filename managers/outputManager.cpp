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
#include "../projectconfiguration.hpp"

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
	    taskYIELD();
	}
}

//! =============================================================================
//! \copyright
//! ===================== end of file: outputmanager.cpp ========================
