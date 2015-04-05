// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#ifndef _OUTPUTEVALUATOR_HPP_
#define _OUTPUTEVALUATOR_HPP_

#include "inputtask.hpp"
#include "inputevaluator.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace outbound {

class OutputEvaluator {
protected: enum OutputState {
	Active,
	Passive,
	TimeOutOn,
	TimeOutOff,
	ActiveUp,
	ActiveDown,
	PassiveUp,
	PassiveDown,
	TimeoutUp,
	TimeoutDown,
	StoppedUp,
	StoppedDown
};

public: inline OutputEvaluator(const u16 hwAddress, const u16 timeoutON, const u16 timeoutOFF, const std::vector<u16>& inputs, std::vector<u32>& data);
public: inline virtual void evaluate();
public: inline cu16 time() const;

private: inline void write();
private: inline void evaluateBranchActive();
private: inline void evaluateBranchPassive();
private: inline void evaluateBranchTimeOutOn();
private: inline void evaluateBranchTimeOutOff();

protected: u16 						_timer;
protected: OutputState 				_state;
protected: std::vector<u32>& 		_data;
protected: cu16 					_hwAddress;
protected: cu16 					_timeoutON;
protected: cu16 					_timeoutOFF;
protected: const std::vector<u16>	_inputs;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline OutputEvaluator::OutputEvaluator(const u16 hwAddress, const u16 timeoutON, const u16 timeoutOFF, const std::vector<u16>& inputs, std::vector<u32>& data):
		_timer(0),
		_state(Passive),
		_data(data),
		_hwAddress(hwAddress),
		_timeoutON(timeoutON),
		_timeoutOFF(timeoutOFF),
		_inputs(inputs) {}

inline cu16 OutputEvaluator::time() const {
	return _timer;
}

inline void OutputEvaluator::write() {
	_data[_hwAddress / (sizeof(_data[0]) * 8)] ^= (1 << (_hwAddress % (sizeof(_data[0]) * 8)));
}

inline void OutputEvaluator::evaluate() {
	switch (_state) {
	case Active: {
		evaluateBranchActive();
		return;
	}
	case Passive: {
		evaluateBranchPassive();
		return;
	}
	case TimeOutOn: {
		evaluateBranchTimeOutOn();
		return;
	}
	case TimeOutOff: {
		evaluateBranchTimeOutOff();
		return;
	}
	default: {
		UARTprintf("Unsupported switch case in Output::evaluate()\n");
		break;
	}
	}
}

inline void OutputEvaluator::evaluateBranchActive() {
	const std::vector<service::inbound::InputEvaluator*>& inputs = service::inbound::InputTaskFactory::get()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		if(service::inbound::NoChangeEvent != inputs[_inputs[index]]->changed()) {
			if(0 != _timeoutOFF) {
				_timer = _timeoutOFF;
				_state = TimeOutOff;
			}
			else {
				write();
				_state = Passive;
			}

			return;
		}
	}
}

inline void OutputEvaluator::evaluateBranchPassive() {
	const std::vector<service::inbound::InputEvaluator*>& inputs = service::inbound::InputTaskFactory::get()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		switch (inputs[_inputs[index]]->changed()) {
		case service::inbound::NoChangeEvent: {
			break;
		}
		case service::inbound::DeferredChangeEvent: {
			if(0 != _timeoutON) {
				_timer = _timeoutON;
				_state = TimeOutOn;
				return;
			}

			break;
		}
		case service::inbound::ChangeEvent: {
			write();
			_state = Active;
			return;
		}
		default:
			UARTprintf("Unsupported switch case in Output::evaluateBranchPassive()\n");
			return;
		}
	}
}

inline void OutputEvaluator::evaluateBranchTimeOutOn() {
	const std::vector<service::inbound::InputEvaluator*>& inputs = service::inbound::InputTaskFactory::get()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		switch (inputs[_inputs[index]]->changed()) {
		case service::inbound::NoChangeEvent: {
			break;
		}
		case service::inbound::DeferredChangeEvent: {
			if(0 != _timeoutON) {
				_timer = _timeoutON;
				return;
			}

			break;
		}
		case service::inbound::ChangeEvent: {
			write();
			_state = Active;
			return;
		}
		default:
			UARTprintf("Unsupported switch case in Output::evaluateBranchPassive()\n");
			return;
		}
	}

	_timer--;
	if(0 == _timer) {
		write();
		_state = Active;
	}
}

inline void OutputEvaluator::evaluateBranchTimeOutOff() {
	const std::vector<service::inbound::InputEvaluator*>& inputs = service::inbound::InputTaskFactory::get()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		if(service::inbound::NoChangeEvent != inputs[_inputs[index]]->changed()) {
			_state = Active;
			return;
		}
	}

	_timer--;
	if(0 == _timer) {
		write();
		_state = Passive;
	}
}

} // outbound
} // service

#endif // _OUTPUTEVALUATOR_HPP_
// =============================================================================
//! \file
//! \copyright
// ====================== end of file: outputevaluator.hpp =====================
