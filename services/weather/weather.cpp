// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#include "json.hpp"
#include "lwip/api.h"
#include "weather.hpp"
#include "requestfactory.hpp"

using namespace std;
using namespace service::weather;
using namespace service::weather::configuration;

bool weather::processingReply(netbuf* reply) {
	// =============================================================================
	// Get field "rain":
	// =============================================================================
	pbuf* p = reply->p;
	s8* found = NULL;
	while(p) {
		UARTwrite((s8*)p->payload, p->len);
		found = strstr(static_cast<s8*>(p->payload), "\"rain\":");
		if(found) {break;}

		p = p->next;
	}

	// =============================================================================
	// Go to the end of the field
	// =============================================================================
	while(found) {
		found = strstr(found, "\":");
		if(found) {
			found += 2;
			break;
		}
	}

	// =============================================================================
	// If the request is the forecast, skip the time range
	// =============================================================================
	if(0 == _forecast) {
		while(found) {
			found = strstr(found, "\":");
			if(found) {
				found += 2;
				break;
			}
		}
	}

	// =============================================================================
	// Put the integer part of the value into the statistic
	// =============================================================================
	_statistic[_forecast] = static_cast<u32>(atof(found));
	netbuf_delete(reply);
	_forecast++;
	if(_statistic.size() < _forecast) {
		_forecast = 0;
	}

	return true;
}

netbuf* weather::generateRequest() {
	netbuf* buf = netbuf_new();
	_requestFactory.reset();
	const string& request = _requestFactory.request("Budapest,HU", _forecast);
	netbuf_ref(buf, request.data(), request.length());

	return buf;
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: weather.cpp =========================
