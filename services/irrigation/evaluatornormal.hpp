// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.28.2015.
//! \note
// =============================================================================
#ifndef _EVALUATORNORMAL_H_
#define _EVALUATORNORMAL_H_

#include "../projectconfiguration.hpp"

namespace service {
namespace irrigation {

class EvaluatorNormal {
protected: enum CircleState {
	Active,
	Passive,
	Wait
};

public: inline EvaluatorNormal(cu32 startTime, cu16 upTime, cu16 downTime, cu16 input, cu16 repeatCount, const tm& currentTime);
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

inline EvaluatorNormal::EvaluatorNormal(cu32 startTime, cu16 upTime, cu16 downTime, cu16 input, cu16 repeatCount, const tm& currentTime):
				_count(0),
				_day(0),
				_state(Passive),
				_startTime(startTime),
				_upTime(upTime),
				_downTime(downTime),
				_input(input),
				_repeatCount(repeatCount),
				_currentTime(currentTime) {}

inline cu16 EvaluatorNormal::time() const {
	return _timer;
}

inline bool EvaluatorNormal::evaluate() {
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

inline bool EvaluatorNormal::evaluateBranchActive() {
	return false;
}

inline bool EvaluatorNormal::evaluateBranchPassive() {
	return false;
}

inline bool EvaluatorNormal::evaluateBranchWait() {
	return false;
}

} // irrigation
} // service

#endif // _EVALUATORNORMAL_H_
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: evaluatornormal.hpp ======================
