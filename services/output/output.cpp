// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \note
// =============================================================================
#include "output.hpp"
#include "../projectconfiguration.hpp"

using namespace manager::output;
using namespace manager::project;
using namespace service::outbound;
using namespace service::outbound::configuration;

DECLARE_TH(output)

output::output():
		_dataByteCount(ProjectManager::getInstance()->sysConfig().hwOutputNumber() / 8),
		_outputManager(OutputManager::getInstance()) {
	_THQueue = xSemaphoreCreateBinary();

	timerStart();
	IOStart();
}

void output::task(void *pvParameters) {
	while(1) {
		// The thread gives up its time-slice, if there is no semaphire given.
		xSemaphoreTake(_THQueue, portMAX_DELAY);

		const std::vector<Output*>& out = _outputManager->outputs();
		for(u32 index = 0; index < out.size(); index++) {
			out[index]->evaluate();
		}

		IOWrite();

//		UARTprintf("D\n");


		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void output::timerStart() const {
	SysCtlPeripheralEnable(timerPeriphery);
	TimerConfigure	(timer, TIMER_CFG_PERIODIC);
	TimerLoadSet	(timer, TIMER_A, systemGlobal::requestedSystemClockFrequency / pollingFrequency);
	TimerIntRegister(timer, TIMER_A, &handlerTH);
	TimerIntEnable	(timer, TIMER_TIMA_TIMEOUT);
	TimerEnable		(timer, TIMER_A);
}

void output::IOStart() const {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);

    GPIOPinConfigure(GPIO_PB4_SSI1FSS); 		//CS
    GPIOPinConfigure(GPIO_PB5_SSI1CLK); 		//CLK
    GPIOPinConfigure(GPIO_PE4_SSI1XDAT0);		//MOSI
    GPIOPinConfigure(GPIO_PE5_SSI1XDAT1);		//MISO

    GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_2);
    LoadOFF();
    GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_2 ,GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_1);

	SSIConfigSetExpClk(SSI1_BASE, systemGlobal::requestedSystemClockFrequency, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 500, 8);
	SSIEnable(SSI1_BASE);
	LoadON();
}

void output::handlerTH() {
	TimerIntClear(timer, TIMER_TIMA_TIMEOUT);
	xSemaphoreGiveFromISR(_THQueue, NULL);
}

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: output.cpp ===========================
