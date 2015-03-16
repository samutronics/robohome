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
	TimeOutOff
};

public: inline Output(const u16 hwAddress, const u16 timeoutON, const u16 timeoutOFF, const std::vector<u16>& inputs, std::vector<u32>& data);
public: inline virtual void evaluate();

protected: inline void writeBit(cu16 address);

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

inline Output::Output(const u16 hwAddress, const u16 timeoutON, const u16 timeoutOFF, const std::vector<u16>& inputs, std::vector<u32>& data):
		_timer(0),
		_state(Passive),
		_data(data),
		_hwAddress(hwAddress),
		_timeoutON(timeoutON),
		_timeoutOFF(timeoutOFF),
		_inputs(inputs) {}

inline void Output::writeBit(cu16 address) {
	_data[address / sizeof(_data[0])] ^= (1 << (address % (sizeof(_data[0]) * 8)));
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

inline void Output::evaluateBranchActive() {
	const std::vector<input::Input*>& inputs = input::InputManager::getInstance()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		if(input::NoChangeEvent != inputs[_inputs[index]]->changed()) {
			if(0 != _timeoutOFF) {
				_timer = _timeoutOFF;
				_state = TimeOutOff;
			}
			else {
				writeBit(_hwAddress);
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
			_timer = _timeoutON;
			_state = TimeOutOn;
			return;
		}
		case input::ChangeEvent: {
			writeBit(_hwAddress);
			_state = Active;
			return;
		}
		default:
			UARTprintf("Unsupported switch case in Output::evaluateBranchPassive()\n");
			return;
		}
	}
}

inline void Output::evaluateBranchTimeOutOn() {
	_timer--;
	if(0 == _timer) {_state = Active;}

	const std::vector<input::Input*>& inputs = input::InputManager::getInstance()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		bool breakable = true;
		switch (inputs[_inputs[index]]->changed()) {
		case input::NoChangeEvent: {
			breakable = false;
			break;
		}
		case input::DeferredChangeEvent: {
			_timer = _timeoutON;
			_state = TimeOutOn;
			break;
		}
		case input::ChangeEvent: {
			_timer = 0;
			_state = Active;
			break;
		}
		default:
			UARTprintf("Unsupported switch case in Output::evaluateBranchTimeOutOn()\n");
			return;
		}

		if(breakable) {break;}
	}

	if(Active == _state) {writeBit(_hwAddress);}
}

inline void Output::evaluateBranchTimeOutOff() {
	_timer--;
	if(0 == _timer) {_state = Passive;}

	const std::vector<input::Input*>& inputs = input::InputManager::getInstance()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		if(input::NoChangeEvent != inputs[_inputs[index]]->changed()) {
			_state = Active;
			break;
		}
	}

	if(_state == Passive) {writeBit(_hwAddress);}
}

} // output
} // manager

#endif // _OUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// =========================== end of file: output.hpp =========================
