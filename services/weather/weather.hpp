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
#include "iservice.hpp"
#include "requestfactory.hpp"
#include "../projectconfiguration.hpp"
#include "abstractservicerequesttransmitter.hpp"

namespace service {
namespace weather {

class weather: public IService, public communication::asbtractServiceRequestTransmitter {
	TO_BE_RUNABLE(weather)

protected: virtual bool processingReply(netbuf* reply);
protected: virtual netbuf* generateRequest();

// =============================================================================
//! \brief There is no tasks assigned to the constructor, but it has to be
//! declared as private.
// =============================================================================
private: weather();
// =============================================================================
//! \brief Manage network connection
//!
//! The task manages the client - server HTTP communication.
// =============================================================================
private: virtual void task(void *pvParameters);

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
