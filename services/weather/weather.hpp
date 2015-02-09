// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _WEATHER_H_
#define _WEATHER_H_

#include "report.hpp"
#include "requestfactory.hpp"
#include "abstractclientservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace weather {

// =============================================================================
//! \brief Service implementation to gather weather informations for the system.
// =============================================================================
class weather: public abstractclientservice {
	TO_BE_RUNABLE(weather)

protected: virtual bool processingReply(netbuf* reply);
protected: virtual netbuf* generateRequest();

// =============================================================================
//! \brief There is no tasks assigned to the constructor, but it has to be
//! declared as private.
// =============================================================================
private: weather();

// =============================================================================
// Member declarations
// =============================================================================
private: weatherRequestFactory	_requestFactory;
};

} // weather

} // service

#endif // _WEATHER_H_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: weather.hpp ===========================
