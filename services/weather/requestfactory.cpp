// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#include "requestfactory.hpp"

using namespace service::weather;

const s8 weatherRequestTemplate[]			= "GET http://api.openweathermap.org/data/2.5/weather?q=";
const s8 weatherRequestForecastTempalte[]	= "GET http://api.openweathermap.org/data/2.5/forecast/daily?q=";
const s8 modeTemplate[]						= "&mode=json&units=metric";
const s8 APPIDOpenWeatherTemplate[]			= "&APIID=afc5370fef1dfec1666a5676346b163b";
const s8 HTTP11Template[]					= " HTTP/1.0\r\n\r\n";
const s8 pcCountTemplate[]					= "&cnt=1";

void weatherRequestFactory::request(weatherRequest& request, const char* location, bool forecast, u32 days) {
	memset(request.request, 0, sizeof(request.request));
	request.len = 0;

	if(strstr(location, " ")) {return;}

	if(forecast) {
		strncpy(request.request + request.len, weatherRequestForecastTempalte, sizeof(weatherRequestForecastTempalte));
		request.len += sizeof(weatherRequestForecastTempalte) - 1;
		request.type = forecastRequest;
	}
	else {
		strncpy(request.request + request.len, weatherRequestTemplate, sizeof(weatherRequestTemplate));
		request.len += sizeof(weatherRequestTemplate) - 1;
		request.type = currentRequest;
	}

	u32 lcoationSize = strlen(location);
	strncpy(request.request + request.len, location, lcoationSize);
	request.len += lcoationSize;

	strncpy(request.request + request.len, modeTemplate, sizeof(modeTemplate));
	request.len += sizeof(modeTemplate) - 1;

	if(forecast) {
		strncpy(request.request + request.len, pcCountTemplate, sizeof(pcCountTemplate));
		request.len += sizeof(pcCountTemplate) - 1;
	}

	strncpy(request.request + request.len, APPIDOpenWeatherTemplate, sizeof(APPIDOpenWeatherTemplate));
	request.len += sizeof(APPIDOpenWeatherTemplate) - 1;

	strncpy(request.request + request.len, HTTP11Template, sizeof(HTTP11Template));
	request.len += sizeof(HTTP11Template) - 1;
}

// =============================================================================
//! \file
//! \copyright
// ==================== end of file: requestfactory.cpp ========================
