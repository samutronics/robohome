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

protected: u16			_count;
protected: u16			_timer;
protected: u8			_day;
protected: CircleState	_state;
protected: cu32			_startTime;
protected: cu16			_upTime;
protected: cu16			_downTime;
protected: cu16			_input;
protected: cu16			_repeatCount;
protected: const tm&	_currentTime;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline EvaluatorGrowm::EvaluatorGrowm(const manager::project::metaIrrigation& irr, const tm& currentTime):
				_count(0),
				_day(std::numeric_limits<u8>::max()),
				_state(Passive),
				_startTime(irr.startTime()),
				_upTime(irr.upTime()),
				_downTime(irr.offsetTime() - irr.upTime()),
				_input(irr.input()),
				_repeatCount(irr.repeatCount()),
				_currentTime(currentTime) {}

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
		manager::input::InputManagerFactory::get()->write(_input, 0);
		_count++;
		_timer = _downTime;
		_state = Wait;
		return false;
	}

	return true;
}

inline bool EvaluatorGrowm::evaluateBranchPassive() {
	if(_day == _currentTime.tm_mday) {
		return false;
	}

	cs32 startHour = _startTime / 3600;
	cs32 startMin = (_startTime - startHour * 3600) / 60;
	if((startHour <= _currentTime.tm_hour) || ((startHour == _currentTime.tm_hour) && (startMin <= _currentTime.tm_min))) {
		manager::input::InputManagerFactory::get()->write(_input, 1);
		_count = 0;
		_day = _currentTime.tm_mday;
		_timer = _upTime;
		_state = Active;
		return true;
	}

	return false;
}

inline bool EvaluatorGrowm::evaluateBranchWait() {
	if(_count == _repeatCount) {
		_state = Passive;
		return false;
	}

	_timer--;
	if(0 == _timer) {
		manager::input::InputManagerFactory::get()->write(_input, 1);
		_timer = _upTime;
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
