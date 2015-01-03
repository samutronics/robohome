// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#include "requestfactory.hpp"

using namespace manager::weatherTask;

const char weatherRequestFactory::g_cWeatherRequest[] = "GET http://api.openweathermap.org/data/2.5/weather?q=";
const char weatherRequestFactory::g_cWeatherRequestForecast[] = "GET http://api.openweathermap.org/data/2.5/forecast/daily?q=";
const char weatherRequestFactory::g_cMode[] = "&mode=json&units=metric";
const char weatherRequestFactory::g_cAPPIDOpenWeather[] = "&APIID=afc5370fef1dfec1666a5676346b163b";
const char weatherRequestFactory::g_cHTTP11[] = " HTTP/1.0\r\n\r\n";
const char weatherRequestFactory::pcCount[] = "&cnt=1";

void weatherRequestFactory::request(weatherRequest& request, const char* location, bool forecast, u32 days) {
	memset(request.request, 0, sizeof(request.request));
	request.len = 0;

	if(strstr(location, " ")) {return;}

	if(forecast) {
		strncpy(request.request + request.len, g_cWeatherRequestForecast, sizeof(g_cWeatherRequestForecast));
		request.len += sizeof(g_cWeatherRequestForecast) - 1;
		request.type = forecastRequest;
	}
	else {
		strncpy(request.request + request.len, g_cWeatherRequest, sizeof(g_cWeatherRequest));
		request.len += sizeof(g_cWeatherRequest) - 1;
		request.type = currentRequest;
	}

	u32 lcoationSize = strlen(location);
	strncpy(request.request + request.len, location, lcoationSize);
	request.len += lcoationSize;

	strncpy(request.request + request.len, g_cMode, sizeof(g_cMode));
	request.len += sizeof(g_cMode) - 1;

	if(forecast) {
		strncpy(request.request + request.len, pcCount, sizeof(pcCount));
		request.len += sizeof(pcCount) - 1;
	}

	strncpy(request.request + request.len, g_cAPPIDOpenWeather, sizeof(g_cAPPIDOpenWeather));
	request.len += sizeof(g_cAPPIDOpenWeather) - 1;

	strncpy(request.request + request.len, g_cHTTP11, sizeof(g_cHTTP11));
	request.len += sizeof(g_cHTTP11) - 1;
}

// =============================================================================
//! \file
//! \copyright
// ================ end of file: weatherrequestfactory.cpp =====================
