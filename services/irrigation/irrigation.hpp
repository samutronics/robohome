// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.27.2015.
//! \note
// =============================================================================
#ifndef _IRRIGATION_H_
#define _IRRIGATION_H_

#include "iinform.hpp"
#include "iservice.hpp"
#include "evaluatorgrowm.hpp"
#include "evaluatornormal.hpp"
#include "singletonfactory.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace irrigation {

class Irrigation: public libs::IInform {
protected: Irrigation();
public: void task(void *pvParameters);

public: virtual bool write(const libs::CommandsIterator& it);
public: virtual bool read(const libs::CommandsIterator& it, std::string& result) const;

private: void timerStart() const;
private: EvaluatorGrowm* evaluatorFactory(const manager::project::metaIrrigation& irr);

private: tm										_currentTime;
private: std::vector<EvaluatorGrowm*>			_evaluators;
private: std::vector<EvaluatorGrowm*>::iterator	_active;

DEFINE_TH
};

typedef libs::SingletonFactory<Irrigation> IrrigationFactory;

// =============================================================================
// Inline method implementation
// =============================================================================

} // irrigation
} // service

#endif // _IRRIGATION_H_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: irrigation.hpp ========================
