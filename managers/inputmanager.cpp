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

using namespace manager::task;
using namespace manager::configuration::inboundTask;

DECLARE_TH(inputManager)

inputManager::inputManager() {
	_THQueue = xQueueCreate(THQueueLength, THQueueWidth);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
    GPIOIntRegister(GPIO_PORTJ_BASE, &inputManager::handlerTH);
    GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
}

void inputManager::task(void *pvParameters) {
	while(1) {
		if(0 == uxQueueMessagesWaiting(_THQueue)) {taskYIELD();}

		u8 state;
		xQueueReceive(_THQueue, &state, 0);

		taskYIELD();
	}
}

void inputManager::handlerTH() {
	static bool loggedState = false;
	loggedState = !loggedState;
	xQueueSendToBackFromISR(_THQueue, &loggedState, NULL);
	GPIOIntClear(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
}

//! =============================================================================
//! \copyright
//! ==================== end of file: inputmanager.hpp ==========================
