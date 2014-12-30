// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _WEATHERREQUESTFACTORY_HPP_
#define _WEATHERREQUESTFACTORY_HPP_

#include <string.h>

#include "../projectconfiguration.hpp"

namespace manager {
namespace weatherTask {

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

private: static const char g_cWeatherRequest[];
private: static const char g_cWeatherRequestForecast[];
private: static const char g_cMode[];
private: static const char g_cAPPIDOpenWeather[];
private: static const char g_cHTTP11[];
private: static const char pcCount[];

};

} // weatherTask

} // manager

#endif // _WEATHERREQUESTFACTORY_HPP_
// =============================================================================
//! \file
//! \copyright
// ================ end of file: weatherrequestfactory.hpp =====================