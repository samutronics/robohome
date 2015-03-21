// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#ifndef _OUTPUT_HPP_
#define _OUTPUT_HPP_

#include "input.hpp"
#include "inputmanager.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace output {

class Output {
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

public: inline Output(const u16 hwAddress, const u16 timeoutON, const u16 timeoutOFF, const std::vector<u16>& inputs, std::vector<u32>& data);
public: inline virtual void evaluate();

private: inline void write();
private: inline void evaluateBranchActive();
private: inline void evaluateBranchPassive();
private: template<bool on> inline void evaluateBranchesTimeOut();

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

inline Output::Output(const u16 hwAddress, const u16 timeoutON, const u16 timeoutOFF, const std::vector<u16>& inputs, std::vector<u32>& data):
		_timer(0),
		_state(Passive),
		_data(data),
		_hwAddress(hwAddress),
		_timeoutON(timeoutON),
		_timeoutOFF(timeoutOFF),
		_inputs(inputs) {}

inline void Output::write() {
	_data[_hwAddress / (sizeof(_data[0]) * 8)] ^= (1 << (_hwAddress % (sizeof(_data[0]) * 8)));
}

inline void Output::evaluate() {
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
		evaluateBranchesTimeOut<true>();
		return;
	}
	case TimeOutOff: {
		evaluateBranchesTimeOut<false>();
		return;
	}
	default: {
		UARTprintf("Unsupported switch case in Output::evaluate()\n");
		break;
	}
	}
}

inline void Output::evaluateBranchActive() {
	const std::vector<input::Input*>& inputs = input::InputManager::getInstance()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		if(input::NoChangeEvent != inputs[_inputs[index]]->changed()) {
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

inline void Output::evaluateBranchPassive() {
	const std::vector<input::Input*>& inputs = input::InputManager::getInstance()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		switch (inputs[_inputs[index]]->changed()) {
		case input::NoChangeEvent: {
			break;
		}
		case input::DeferredChangeEvent: {
			if(0 != _timeoutON) {
				_timer = _timeoutON;
				_state = TimeOutOn;
				return;
			}

			break;
		}
		case input::ChangeEvent: {
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

template<bool on> inline void Output::evaluateBranchesTimeOut() {
	const std::vector<input::Input*>& inputs = input::InputManager::getInstance()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		if(input::NoChangeEvent != inputs[_inputs[index]]->changed()) {
			_state = on ? Passive : Active;
			return;
		}
	}

	_timer--;
	if(0 == _timer) {
		write();
		_state = on ? Active : Passive;
	}
}

} // output
} // manager

#endif // _OUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// =========================== end of file: output.hpp =========================
