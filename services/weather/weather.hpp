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
#include "singletonfactory.hpp"
#include "abstractclientservice.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace weather {

// =============================================================================
//! \brief Service implementation to gather weather informations for the system.
// =============================================================================
class Weather: public abstractclientservice {
public: inline const std::vector<u32>& statistic() const;

protected: virtual bool processingReply(netbuf* reply);
protected: virtual netbuf* generateRequest();

// =============================================================================
//! \brief There is no tasks assigned to the constructor, but it has to be
//! declared as private.
// =============================================================================
protected: inline Weather();

// =============================================================================
// Member declarations
// =============================================================================
private: u32				 	_forecast;
private: std::vector<u32>		_statistic;
private: weatherRequestFactory	_requestFactory;
};

typedef libs::SingletonFactory<Weather> WeatherFactory;

// =============================================================================
// Inline method implementation
// =============================================================================
inline Weather::Weather():
		abstractclientservice(configuration::url, configuration::port, NETCONN_TCP, configuration::updatePeriode),
		_forecast(0),
		_statistic(3, 0) {}

inline const std::vector<u32>& Weather::statistic() const {
	return _statistic;
}

} // weather
} // service

#endif // _WEATHER_H_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: weather.hpp ===========================
