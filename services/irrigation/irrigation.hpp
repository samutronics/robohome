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
#include "metairrigation.hpp"
#include "evaluatornormal.hpp"
#include "singletonfactory.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace irrigation {

class irrigation: public libs::IInform {
protected: irrigation();
public: void task(void *pvParameters);

public: virtual bool write(const libs::CommandsIterator& it);
public: virtual bool read(const libs::CommandsIterator& it, std::string& result) const;

private: void timerStart() const;
private: EvaluatorNormal* evaluatorFactory(const manager::project::metaIrrigation& irr);

private: tm											_currentTime;
private: std::vector<EvaluatorNormal*>				_evaluators;
private: std::vector<EvaluatorNormal*>::iterator	_active;

DEFINE_TH
};

typedef libs::SingletonFactory<irrigation> IrrigationFactory;

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
