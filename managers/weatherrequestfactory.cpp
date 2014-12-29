// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================

#include "weatherrequestfactory.hpp"

using namespace manager::weatherTask;

const char weatherRequestFactory::g_cWeatherRequest[] = "GET http://api.openweathermap.org/data/2.5/weather?q=";
const char weatherRequestFactory::g_cWeatherRequestForecast[] = "GET http://api.openweathermap.org/data/2.5/forecast/daily?q=";
const char weatherRequestFactory::g_cMode[] = "&mode=json&units=metric";
const char weatherRequestFactory::g_cAPPIDOpenWeather[] = "&APIID=afc5370fef1dfec1666a5676346b163b";
const char weatherRequestFactory::g_cHTTP11[] = " HTTP/1.0\r\n\r\n";
const char weatherRequestFactory::pcCount[] = "&cnt=1";

const weatherRequest& weatherRequestFactory::request(const char* location, bool forecast, u32 days) {
	static weatherRequest rq;
	memset(rq.request, 0, sizeof(rq.request));
	rq.len = 0;

	if(strstr(location, " ")) {
		return rq;
	}

	if(forecast) {
		strncpy(rq.request + rq.len, g_cWeatherRequestForecast, sizeof(g_cWeatherRequestForecast));
		rq.len += sizeof(g_cWeatherRequestForecast);
	}
	else {
		strncpy(rq.request + rq.len, g_cWeatherRequest, sizeof(g_cWeatherRequest));
		rq.len += sizeof(g_cWeatherRequest);
	}

	u32 lcoationSize = strlen(location);
	strncpy(rq.request + rq.len, location, lcoationSize);
	rq.len += lcoationSize;

	strncpy(rq.request + rq.len, g_cMode, sizeof(g_cMode));
	rq.len += sizeof(g_cMode);

	if(forecast) {
		strncpy(rq.request + rq.len, pcCount, sizeof(pcCount));
		rq.len += sizeof(pcCount);
	}

	strncpy(rq.request + rq.len, g_cAPPIDOpenWeather, sizeof(g_cAPPIDOpenWeather));
	rq.len += sizeof(g_cAPPIDOpenWeather);

	strncpy(rq.request + rq.len, g_cHTTP11, sizeof(g_cHTTP11));
	rq.len += sizeof(g_cHTTP11);

	return rq;
}

// =============================================================================
//! \file
//! \copyright
// ================ end of file: weatherrequestfactory.cpp =====================
