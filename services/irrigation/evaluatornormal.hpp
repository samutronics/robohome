// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.28.2015.
//! \note
// =============================================================================
#ifndef _EVALUATORNORMAL_H_
#define _EVALUATORNORMAL_H_

#include "weather.hpp"
#include "evaluatorgrowm.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace irrigation {

class EvaluatorNormal: public EvaluatorGrowm {
public: inline EvaluatorNormal(const manager::project::metaIrrigation& irr, const tm& currentTime);

protected: virtual inline bool evaluateBranchActive();
protected: virtual inline bool evaluateBranchPassive();

private: inline cu32 evaluateWeather();

private: std::vector<u16>	_precipitation;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline EvaluatorNormal::EvaluatorNormal(const manager::project::metaIrrigation& irr, const tm& currentTime):
		EvaluatorGrowm(irr, currentTime),
		_precipitation(6, 0) {
	for(u32 index = 0; index < 4; index++) {
		_precipitation[index] = irr.dailyPrecipitation();
	}
}

inline bool EvaluatorNormal::evaluateBranchActive() {
	_timer--;
	if(0 == _timer) {
		manager::input::InputManagerFactory::get()->write(_descriptor.input(), 0);
		_state = Passive;
		return false;
	}

	return true;
}

inline bool EvaluatorNormal::evaluateBranchPassive() {
	if(_day == _currentTime.tm_mday) {
		return false;
	}

	cs32 startHour = _descriptor.startTime() / 3600;
	cs32 startMin = (_descriptor.startTime() - startHour * 3600) / 60;
	if((startHour <= _currentTime.tm_hour) || ((startHour == _currentTime.tm_hour) && (startMin <= _currentTime.tm_min))) {
		_day = _currentTime.tm_mday;
		cu32 time = evaluateWeather();
		if(0 != time) {
			manager::input::InputManagerFactory::get()->write(_descriptor.input(), 1);
			_timer = time;
			_state = Active;
			return true;
		}
	}

	return false;
}

inline cu32 EvaluatorNormal::evaluateWeather() {
	// =============================================================================
	//! The indexed item in the overal statistic
	// =============================================================================
	cu32 today = 3;

	// =============================================================================
	//! Shift the dispensed precipitation window
	// =============================================================================
	for(u32 index = 0; index < _precipitation.size() - 1; index++) {
		_precipitation[index] = _precipitation[index + 1];
	}

	// =============================================================================
	//! Update the forecasted days
	// =============================================================================
	const std::vector<u32>& stat = weather::WeatherFactory::get()->statistic();
	for(u32 index = 0; index < stat.size(); index++) {
		_precipitation[today + index] = stat[index] / _descriptor.unitPrecipitation();
	}

	// =============================================================================
	//! Summarize it
	// =============================================================================
	s32 sum = 0;
	for(u32 index = 0; index < _precipitation.size(); index++) {
		sum += _precipitation[index];
	}

	// =============================================================================
	//! Compute the missing precipitation
	// =============================================================================
	s32 current = (static_cast<s32>(_descriptor.dailyPrecipitation() * _precipitation.size()) - sum) / 2;

	// =============================================================================
	//! Limit the current time
	// =============================================================================
	if(current < 0) {
		_precipitation[today] = 0;
	}
	else if(current > _descriptor.upTime() * 2) {
		_precipitation[today] = _descriptor.upTime() * 2;
	}
	else {
		_precipitation[today] = current;
	}

	return _precipitation[today];
}

} // irrigation
} // service

#endif // _EVALUATORNORMAL_H_
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: evaluatornormal.hpp ======================
