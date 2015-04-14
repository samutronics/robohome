// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			04.12.2014.
//! \note
// =============================================================================
#include "httppair.hpp"
#include "mediator.hpp"
#include "inputtask.hpp"
#include "metainput.hpp"
#include "metasysconfig.hpp"
#include "projectmanager.hpp"

using namespace std;
using namespace libs;
using namespace systemGlobal;
using namespace manager::project;
using namespace service::inbound;
using namespace service::inbound::configuration;

DECLARE_TH(InputTask)

InputTask::InputTask(): _inputs(ProjectManagerFactory::get()->input().count(), 0),
_dataChanged((_inputs.size() / (sizeof(_dataChanged[0]) * 8)) + ((_inputs.size() % (sizeof(_dataChanged[0]) * 8)) ? 1 : 0), 0),
_dataCurrent(_dataChanged.size(), 0),
_dataPrevious(_dataChanged.size(), 0),
_transmissionStatus(ProjectManagerFactory::get()->sysConfig().hwInputNumber() / 8, 0),
_lock(_dataChanged.size()) {
	metaInput data = ProjectManagerFactory::get()->input();
	for(u32 index = 0; index < data.count(); index++) {
		_inputs[index] = new InputEvaluator(data.trigger(), index, _dataChanged, _dataCurrent, _dataPrevious);
		data.next();
	}

	for(u32 index = 0; index < _dataChanged.size(); index++) {_lock[index] = xSemaphoreCreateMutex();}

	MediatorFactory::get()->attach(ComponentIDInputService, this);
	_THQueue = xSemaphoreCreateMutex();
	IOStart();
	IOTransmit(_transmissionStatus);
	for(u32 index = 0; index < _transmissionStatus.size(); ++index) {
		_dataCurrent[index / sizeof(_dataCurrent[0])] |= _transmissionStatus[index] << (index % (sizeof(_dataCurrent[0]) * 8));
	}

	timerStart();
}

InputTask::~InputTask() {
	for(u32 index = 0; index < _inputs.size(); index++) {delete _inputs[index];}

	for(u32 index = 0; index < _lock.size(); index++) {vSemaphoreDelete(_lock[index]);}
}

void InputTask::task(void* /*pvParameters*/) {
	while(true) {
		// The thread gives up its time-slice, if there is no semaphore given.
		xSemaphoreTake(_THQueue, portMAX_DELAY);

		for(u32 index = 0; index < _transmissionStatus.size(); _transmissionStatus[index++] = 0);
		IOTransmit(_transmissionStatus);

		write(_transmissionStatus);
	}
}

bool InputTask::write(const CommandsIterator& it) {
	if(CmdWriteInput == (it.key() & CmdInputMask)) {
		write(it.key() & addressMask, strtoul(it.value().c_str(), NULL, 10));
		return true;
	}

	return false;
}

bool InputTask::read(const CommandsIterator& it, std::string& result) const {
	if(CmdReadInput == (it.key() & CmdInputMask)) {
		for(u32 index = 0; index < _transmissionStatus.size(); ++index) {
			result += HttpPair(it.key() | index, _transmissionStatus[index]).pair();
			result += '&';
		}

		result.erase(result.size() - 1);
		return true;
	}

	return false;
}

void InputTask::timerStart() const {
	SysCtlPeripheralEnable(configuration::timerPeriphery);
	TimerConfigure	(configuration::timer, TIMER_CFG_PERIODIC);
	TimerLoadSet	(configuration::timer, TIMER_A, systemGlobal::requestedSystemClockFrequency / configuration::pollingFrequency);
	TimerIntRegister(configuration::timer, TIMER_A, &handlerTH);
	TimerIntEnable	(configuration::timer, TIMER_TIMA_TIMEOUT);
	TimerEnable		(configuration::timer, TIMER_A);
}

void InputTask::IOStart() const {
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

void InputTask::handlerTH() {
	TimerIntClear(timer, TIMER_TIMA_TIMEOUT);
	xSemaphoreGiveFromISR(_THQueue, NULL);
}

// =============================================================================
//! \file
//! \copyright
// ======================== end of file: input.cpp =============================
