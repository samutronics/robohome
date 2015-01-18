// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _REQUESTFACTORY_HPP_
#define _REQUESTFACTORY_HPP_

#include <string.h>

#include "../projectconfiguration.hpp"

namespace service {
namespace weather {

enum requestType {
	currentRequest,
	forecastRequest
};

class weatherRequest {
public: s8 request[configuration::requestSize];
public: u16 len;
public: requestType type;
};

class weatherRequestFactory {

public: static void request(weatherRequest& request, const char* location, bool forecast, u32 days);
};

} // weatherTask

} // service

#endif // _REQUESTFACTORY_HPP_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: requestfactory.hpp ========================
