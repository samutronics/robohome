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

public: inline EvaluatorNormal(cu32 startTime, cu16 upTime, cu16 downTime, cu16 repeatCount, const tm& currentTime);
public: inline bool evaluate();

protected: virtual inline bool evaluateBranchActive();
protected: virtual inline bool evaluateBranchPassive();
protected: virtual inline bool evaluateBranchWait();

protected: u16			_count;
protected: CircleState	_state;
protected: cu32			_startTime;
protected: cu16			_upTime;
protected: cu16			_downTime;
protected: cu16			_repeatCount;
protected: const tm&	_currentTime;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline EvaluatorNormal::EvaluatorNormal(cu32 startTime, cu16 upTime, cu16 downTime, cu16 repeatCount, const tm& currentTime):
				_count(0),
				_state(Passive),
				_startTime(startTime),
				_upTime(upTime),
				_downTime(downTime),
				_repeatCount(repeatCount),
				_currentTime(currentTime) {}

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

}

inline bool EvaluatorNormal::evaluateBranchPassive() {

}

inline bool EvaluatorNormal::evaluateBranchWait() {

}

} // irrigation
} // service

#endif // _EVALUATORNORMAL_H_
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: evaluatornormal.hpp ======================
