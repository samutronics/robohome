// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \note
// =============================================================================
#include "mediator.hpp"
#include "inputtask.hpp"
#include "outputtask.hpp"
#include "projectmanager.hpp"
#include "projectconfiguration.hpp"
#include "tristateoutputevaluator.hpp"

using namespace std;
using namespace libs;
using namespace systemGlobal;
using namespace manager::project;
using namespace service::inbound;
using namespace service::outbound;
using namespace service::outbound::configuration;

DECLARE_TH(OutputTask)

OutputTask::OutputTask(): _dataByteCount(ProjectManagerFactory::get()->sysConfig().hwOutputNumber() / 8),
_data(ProjectManagerFactory::get()->output().count() + ProjectManagerFactory::get()->triStateOutput().count()
		/ (sizeof(_data[0] * 8)) + ((ProjectManagerFactory::get()->output().count() + ProjectManagerFactory::get()->triStateOutput().count() % (sizeof(_data[0]) * 8)) ? 1 : 0), 0) {
	std::vector<u16> simpleTmp;
	metaOutput simpleoutput = ProjectManagerFactory::get()->output();
	for(u32 index = 0; index < simpleoutput.count(); index++) {
		simpleoutput.inputs(simpleTmp);
		_output[simpleoutput.address()] = new OutputEvaluator(simpleoutput.address(), simpleoutput.timeoutON(), simpleoutput.timeoutOFF(), simpleTmp, _data);
		simpleTmp.clear();
		simpleoutput.next();
	}

	std::vector<u16> tristateTmpUp;
	std::vector<u16> tristateTmpDown;
	metaTriStateOutput tristateutput = ProjectManagerFactory::get()->triStateOutput();
	for(u32 index = 0; index < tristateutput.count(); index++) {
		tristateutput.inputs(simpleTmp);
		tristateutput.inputsUp(tristateTmpUp);
		tristateutput.inputsDown(tristateTmpDown);
		_output[tristateutput.address()] = new TriStateOutputEvaluator(
				tristateutput.address(),
				tristateutput.timeoutON(),
				tristateutput.timeoutOFF(),
				simpleTmp,
				_data,
				tristateutput.extendedAddress(),
				tristateTmpUp,
				tristateTmpDown);

		simpleTmp.clear();
		tristateTmpUp.clear();
		tristateTmpDown.clear();
		tristateutput.next();
	}

	MediatorFactory::get()->attach(ComponentIDOutputService, this);

	_THQueue = xSemaphoreCreateBinary();
	timerStart();
	IOStart();
	IOTransmit();
}

OutputTask::~OutputTask() {
	for(std::map<u32, OutputEvaluator*>::iterator it = _output.begin(); it != _output.end(); ++it) {delete it->second;}
}

void OutputTask::task(void *pvParameters) {
	while(true) {
		// The thread gives up its time-slice, if there is no semaphore given.
		xSemaphoreTake(_THQueue, portMAX_DELAY);

		for(std::map<u32, OutputEvaluator*>::const_iterator it = _output.begin(); it != _output.end(); ++it) {
			it->second->evaluate();
		}

		InputTaskFactory::get()->reset();

		IOTransmit();
	}
}

bool OutputTask::write(const CommandsIterator& it) {
	return false;
}

bool OutputTask::read(const CommandsIterator& it, std::string& result) const {
	switch (it.key() & CmdOutputMask) {
	case CmdReadOutput: {
		for(u32 index = 0; index < _data.size(); index++) {
			s8 buf[10];
			sprintf(buf, "%d", _data[index]);
			result += buf;
		}

		return true;
	}
	case CmdReadOutputTime: {
		map<u32, OutputEvaluator*>::const_iterator o = _output.find(it.key() & addressMask);
		if(!o->second) {
			return false;
		}

		s8 buf[10];
		sprintf(buf, "%d", o->second->time());
		result += buf;
		return true;
	}
	default: {
		UARTprintf("Unsupported command OutputManager::read()\n");
		return false;
	}
	}
}

void OutputTask::timerStart() const {
	SysCtlPeripheralEnable(timerPeriphery);
	TimerConfigure	(timer, TIMER_CFG_PERIODIC);
	TimerLoadSet	(timer, TIMER_A, systemGlobal::requestedSystemClockFrequency / pollingFrequency);
	TimerIntRegister(timer, TIMER_A, &handlerTH);
	TimerIntEnable	(timer, TIMER_TIMA_TIMEOUT);
	TimerEnable		(timer, TIMER_A);
}

void OutputTask::IOStart() const {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);

    GPIOPinConfigure(GPIO_PB5_SSI1CLK); 		//CLK
    GPIOPinConfigure(GPIO_PE4_SSI1XDAT0);		//MOSI
    GPIOPinConfigure(GPIO_PE5_SSI1XDAT1);		//MISO

    GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_5);
    GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // controlling the CS pin has to be done manually instead of the SSI peripheria
    // because there are more than 8 output can be present
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_2);
    LoadOFF();
    GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_2 ,GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_1);

	SSIConfigSetExpClk(SSI1_BASE, systemGlobal::requestedSystemClockFrequency, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 500, 8);
	SSIEnable(SSI1_BASE);
	LoadON();
}

void OutputTask::handlerTH() {
	TimerIntClear(timer, TIMER_TIMA_TIMEOUT);
	xSemaphoreGiveFromISR(_THQueue, NULL);
}

// =============================================================================
//! \file
//! \copyright
// ======================= end of file: outputtask.cpp =========================
