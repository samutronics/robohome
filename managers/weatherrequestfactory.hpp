// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _WEATHERREQUESTFACTORY_H_
#define _WEATHERREQUESTFACTORY_H_

#include <string.h>

#include "../projectconfiguration.hpp"

namespace manager {
namespace weatherTask {

class weatherRequest {
public: s8 request[configuration::requestSize];
public: u32 len;
};


class weatherRequestFactory {
public: static const weatherRequest& request(const char* location, bool forecast, u32 days);

private: static const char g_cWeatherRequest[];
private: static const char g_cWeatherRequestForecast[];
private: static const char g_cMode[];
private: static const char g_cAPPIDOpenWeather[];
private: static const char g_cHTTP11[];
private: static const char pcCount[];

};

} // weatherTask

} // manager

#endif // _WEATHERREQUESTFACTORY_H_
// =============================================================================
//! \file
//! \copyright
// ================ end of file: weatherrequestfactory.hpp =====================
