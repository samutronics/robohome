// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#include "requestfactory.hpp"

using namespace std;
using namespace service::weather;

const s8 weatherRequest[]			= "GET http://api.openweathermap.org/data/2.5/weather?q=";
const s8 weatherRequestForecast[]	= "GET http://api.openweathermap.org/data/2.5/forecast/daily?q=";
const s8 mode[]						= "&mode=json&units=metric";
const s8 APPIDOpenWeather[]			= "&APIID=afc5370fef1dfec1666a5676346b163b";
const s8 HTTP11[]					= " HTTP/1.0\r\n\r\n";
const s8 countOfDays[]				= "&cnt=1";

const std::string& weatherRequestFactory::request(const char* location, bool forecast, u32 days) {
	if(!_request.empty()) {
		return _request;
	}

	if(!location) {return _request;}

	if(forecast) {_request.append(weatherRequestForecast);}
	else {_request.append(weatherRequest);}

	_request.append(location);
	_request.append(mode);

	if(forecast) {_request.append(countOfDays);}

	_request.append(APPIDOpenWeather);
	_request.append(HTTP11);
	return _request;
}

// =============================================================================
//! \file
//! \copyright
// ==================== end of file: requestfactory.cpp ========================
