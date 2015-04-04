// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.28.2015.
//! \note
// =============================================================================
#ifndef _EVALUATORGROWN_H_
#define _EVALUATORGROWN_H_

#include "inputmanager.hpp"
#include "metairrigation.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace irrigation {

class EvaluatorGrowm {
protected: enum CircleState {
	Active,
	Passive,
	Wait
};

public: inline EvaluatorGrowm(const manager::project::metaIrrigation& irr, const tm& currentTime);
public: inline bool evaluate();
public: inline cu16 time() const;

protected: virtual inline bool evaluateBranchActive();
protected: virtual inline bool evaluateBranchPassive();
protected: virtual inline bool evaluateBranchWait();

protected: u16			_timer;
protected: u8			_day;
protected: CircleState	_state;
protected: const manager::project::metaIrrigation _descriptor;
protected: const tm&	_currentTime;

private: u16			_count;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline EvaluatorGrowm::EvaluatorGrowm(const manager::project::metaIrrigation& irr, const tm& currentTime):
				_day(std::numeric_limits<u8>::max()),
				_state(Passive),
				_descriptor(irr),
				_currentTime(currentTime),
				_count(0) {}

inline cu16 EvaluatorGrowm::time() const {
	return _timer;
}

inline bool EvaluatorGrowm::evaluate() {
	switch (_state) {
	case Active: {
		return evaluateBranchActive();
	}
	case Passive: {
		return evaluateBranchPassive();
	}
	case Wait: {
		return evaluateBranchWait();
	}
	default: {
		UARTprintf("Unsupported switch case in EvaluatorNormal::evaluate()\n");
		break;
	}
	}

	return false;
}

inline bool EvaluatorGrowm::evaluateBranchActive() {
	_timer--;
	if(0 == _timer) {
		manager::input::InputManagerFactory::get()->write(_descriptor.input(), 0);
		_count++;
		_timer = _descriptor.offsetTime() - _descriptor.upTime();
		_state = Wait;
		return false;
	}

	return true;
}

inline bool EvaluatorGrowm::evaluateBranchPassive() {
	if(_day == _currentTime.tm_mday) {
		return false;
	}

	cs32 startHour = _descriptor.startTime() / 3600;
	cs32 startMin = (_descriptor.startTime() - startHour * 3600) / 60;
	if((startHour <= _currentTime.tm_hour) || ((startHour == _currentTime.tm_hour) && (startMin <= _currentTime.tm_min))) {
		manager::input::InputManagerFactory::get()->write(_descriptor.input(), 1);
		_count = 0;
		_day = _currentTime.tm_mday;
		_timer = _descriptor.offsetTime() - _descriptor.upTime();
		_state = Active;
		return true;
	}

	return false;
}

inline bool EvaluatorGrowm::evaluateBranchWait() {
	if(_count ==  _descriptor.repeatCount()) {
		_state = Passive;
		return false;
	}

	_timer--;
	if(0 == _timer) {
		manager::input::InputManagerFactory::get()->write(_descriptor.input(), 1);
		_timer = _descriptor.upTime();
		_state = Active;
		return true;
	}

	return false;
}

} // irrigation
} // service

#endif // _EVALUATORGROWN_H_
// =============================================================================
//! \file
//! \copyright
// ====================== end of file: evaluatorgrowm.hpp ======================
