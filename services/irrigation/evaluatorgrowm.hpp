// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.28.2015.
//! \note
// =============================================================================
#ifndef _EVALUATORGROWN_H_
#define _EVALUATORGROWN_H_

#include "evaluatornormal.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace irrigation {

class EvaluatorGrowm: public EvaluatorNormal {
public: inline EvaluatorGrowm(cu32 startTime, cu16 upTime, cu16 downTime, cu16 repeatCount, const tm& currentTime);

protected: virtual inline bool evaluateBranchActive();
protected: virtual inline bool evaluateBranchPassive();
protected: virtual inline bool evaluateBranchWait();
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline EvaluatorGrowm::EvaluatorGrowm(cu32 startTime, cu16 upTime, cu16 downTime, cu16 repeatCount, const tm& currentTime):
		EvaluatorNormal(startTime, upTime, downTime, repeatCount, currentTime) {}

inline bool EvaluatorGrowm::evaluateBranchActive() {

}

inline bool EvaluatorGrowm::evaluateBranchPassive() {

}

inline bool EvaluatorGrowm::evaluateBranchWait() {

}

} // irrigation
} // service

#endif // _EVALUATORGROWN_H_
// =============================================================================
//! \file
//! \copyright
// ====================== end of file: evaluatorgrowm.hpp ======================
