//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			04.12.2014.
//! \details
//! \note
//! =============================================================================

#include "inputmanager.hpp"
#include "../projectconfiguration.hpp"

#include <stdint.h>

#define TARGET_IS_TM4C129_RA1

#include "rom.h"

#include "hw_gpio.h"
#include "hw_types.h"
#include "hw_memmap.h"

#include "gpio.h"
#include "sysctl.h"
#include "pin_map.h"

using namespace manager::task;
using namespace manager::configuration::inputTask;

inputManager::~inputManager() {

}

inputManager::inputManager() {
	_queueInbound = xQueueCreate(inboundQueueLength, inboundQueueWidth);

    // PJ0 are used for user buttons
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);

    ROM_GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);

    //TODO: refator: this method uses the SRAM vector table. Registration need in the startup file
//    ROM_GPIOIntRegister(GPIO_PORTJ_BASE, PortAIntHandler);

    ROM_GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
    ROM_GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_PIN_0);

}

void inputManager::task(void *pvParameters) {
	while(1) {

	}
}

//! =============================================================================
//! \copyright
//! ==================== end of file: inputmanager.hpp ==========================
