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
#include "evaluatornormal.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace irrigation {

class EvaluatorGrowm: public EvaluatorNormal {
public: inline EvaluatorGrowm(cu32 startTime, cu16 upTime, cu16 downTime, cu16 input, cu16 repeatCount, const tm& currentTime);

protected: virtual inline bool evaluateBranchActive();
protected: virtual inline bool evaluateBranchPassive();
protected: virtual inline bool evaluateBranchWait();
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline EvaluatorGrowm::EvaluatorGrowm(cu32 startTime, cu16 upTime, cu16 downTime, cu16 input, cu16 repeatCount, const tm& currentTime):
		EvaluatorNormal(startTime, upTime, downTime, input, repeatCount, currentTime) {}

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
