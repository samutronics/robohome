// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.27.2015.
//! \note
// =============================================================================
#include "irrigation.hpp"
#include "projectmanager.hpp"
#include "../projectconfiguration.hpp"

using namespace libs;
using namespace manager::project;
using namespace service::irrigation;
using namespace service::irrigation::configuration;

DECLARE_TH(irrigation);

irrigation::irrigation():
				_evaluators(ProjectManagerFactory::get()->irrigation().count(), 0),
				_active(_evaluators.begin()) {
	metaIrrigation irr = ProjectManagerFactory::get()->irrigation();
	for(u32 index = 0; index < irr.count(); index++) {
		_evaluators[index] = evaluatorFactory(irr);
		irr.next();
	}

	_THQueue = xSemaphoreCreateBinary();
	timerStart();
}

void irrigation::task(void *pvParameters) {
	while(true) {
		// The thread gives up its time-slice, if there is no semaphore given.
		xSemaphoreTake(_THQueue, portMAX_DELAY);

		HibernateCalendarGet(&_currentTime);
		_currentTime.tm_hour++;

		for(_active = ((_active == _evaluators.end()) ? _evaluators.begin() : _active); _active != _evaluators.end(); ++_active) {
			if((*_active)->evaluate()) {break;}
		}
	}
}

EvaluatorNormal* irrigation::evaluatorFactory(const metaIrrigation& irr) {
	switch (irr.mode()) {
	case Normal: {
		return new EvaluatorNormal(irr.startTime(), irr.upTime(), irr.offsetTime() - irr.upTime(), irr.input(), irr.repeatCount(), _currentTime);
	}
	case Grown: {
		return new EvaluatorGrowm(irr.startTime(), irr.upTime(), irr.offsetTime() - irr.upTime(), irr.input(), irr.repeatCount(), _currentTime);
	}
	default: {
		UARTprintf("Unsuported evalator type in irrigation::evaluatorFactory()\n");
	}
	}

	return NULL;
}

void irrigation::timerStart() const {
	SysCtlPeripheralEnable(timerPeriphery);
	TimerConfigure	(timer, TIMER_CFG_PERIODIC);
	TimerLoadSet	(timer, TIMER_A, systemGlobal::requestedSystemClockFrequency / pollingFrequency);
	TimerIntRegister(timer, TIMER_A, &handlerTH);
	TimerIntEnable	(timer, TIMER_TIMA_TIMEOUT);
	TimerEnable		(timer, TIMER_A);
}

bool irrigation::write(const CommandsIterator& it) {
	return false;
}

bool irrigation::read(const CommandsIterator& it, std::string& result) const {
	if((CmdTimer == (it.key() & CmdIrrigationMask)) && (it.key() & addressMask < _evaluators.size())) {
		s8 buf[10];
		sprintf(buf, "%d", _evaluators[it.key() & addressMask]->time());
		result += buf;
		return true;
	}

	return false;
}


void irrigation::handlerTH() {
	TimerIntClear(timer, TIMER_TIMA_TIMEOUT);
	xSemaphoreGiveFromISR(_THQueue, NULL);
}

// =============================================================================
//! \file
//! \copyright
// ======================= end of file: irrigation.cpp =========================
