// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			04.12.2014.
//! \note
// =============================================================================
#include "input.hpp"
#include "projectmanager.hpp"
#include "../projectconfiguration.hpp"

using namespace manager::input;
using namespace manager::project;
using namespace service::inbound;
using namespace service::inbound::configuration;

xSemaphoreHandle input::_ISRQueue = NULL;
input* input::_instance = NULL;

input::input():
		_dataByteCount(ProjectManager::getInstance()->sysConfig().hwInputNumber() / 8),
		_data(_dataByteCount / sizeof(_data[0]) + (_dataByteCount % sizeof(_data[0]) ? 1 : 0), 0),
		_iputManager(InputManager::getInstance()) {
	IOStart();
	timerStart();
}

void input::task(void *pvParameters) {
	while(true) {
		// The thread gives up its time-slice, if there is no semaphore given.
		xSemaphoreTake(_ISRQueue, portMAX_DELAY);

		IORead();

		_iputManager->write(_data);

		for(u32 index = 0; index < _data.size(); _data[index++] = 0);

		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void input::timerStart() const {
	SysCtlPeripheralEnable(configuration::timerPeriphery);
	TimerConfigure	(configuration::timer, TIMER_CFG_PERIODIC);
	TimerLoadSet	(configuration::timer, TIMER_A, systemGlobal::requestedSystemClockFrequency / configuration::pollingFrequency);
	TimerIntRegister(configuration::timer, TIMER_A, &ISRHandler);
	TimerIntEnable	(configuration::timer, TIMER_TIMA_TIMEOUT);
	TimerEnable		(configuration::timer, TIMER_A);
}

void input::IOStart() const {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);

    GPIOPinConfigure(GPIO_PA2_SSI0CLK); 		//CLK
    GPIOPinConfigure(GPIO_PA3_SSI0FSS); 		//CS
    GPIOPinConfigure(GPIO_PA5_SSI0XDAT1);		//MISO

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5);

    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_0 ,GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);

	SSIConfigSetExpClk(SSI0_BASE, systemGlobal::requestedSystemClockFrequency, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000, 8);
	SSIEnable(SSI0_BASE);
}

void input::ISRHandler() {
	TimerIntClear(timer, TIMER_TIMA_TIMEOUT);
	xSemaphoreGiveFromISR(_ISRQueue, NULL);
}

// =============================================================================
//! \file
//! \copyright
// ======================== end of file: input.cpp =============================
