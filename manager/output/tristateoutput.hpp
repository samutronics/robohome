// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#ifndef _TRISTATEOUTPUT_HPP_
#define _TRISTATEOUTPUT_HPP_

#include "output.hpp"
#include "projectconfiguration.hpp"

namespace manager {
namespace output {

class TriStateOutput: public Output {
public: inline TriStateOutput(cu16 hwAddress, cu16 timeoutON, cu16 timeoutOFF, const std::vector<u16>& inputsUpDown, std::vector<u32>& data, cu16 extendedAddress, const std::vector<u16>& inputsUp, const std::vector<u16>& inputsDown);

public: inline virtual void evaluate();

private: inline void stop();
private: inline void moveUp();
private: inline void moveDown();

private: template<bool up> inline void evaluateBranchesActive();
private: template<bool up> inline void evaluateBranchesPassive();
private: template<bool up> inline void evaluateBranchesTimeout();
private: template<bool up> inline void evaluateBranchesStopped();

private: cu16					_extendedAddress;
private: const std::vector<u16>	_inputsUp;
private: const std::vector<u16>	_inputsDown;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline TriStateOutput::TriStateOutput(cu16 hwAddress, cu16 timeoutON, cu16 timeoutOFF, const std::vector<u16>& inputsUpDown, std::vector<u32>& data, cu16 extendedAddress, const std::vector<u16>& inputsUp, const std::vector<u16>& inputsDown):
						Output(hwAddress, timeoutON, timeoutOFF, inputsUpDown, data),
						_extendedAddress(extendedAddress),
						_inputsUp(inputsUp),
						_inputsDown(inputsDown) {
	_state = PassiveUp;
}

inline void TriStateOutput::stop() {
	_data[_hwAddress / (sizeof(_data[0]) * 8)] &= ~(1 << (_hwAddress % (sizeof(_data[0]) * 8)));
	_data[_extendedAddress / (sizeof(_data[0]) * 8)] &= ~(1 << (_extendedAddress % (sizeof(_data[0]) * 8)));
}

inline void TriStateOutput::moveUp() {
	_data[_hwAddress / (sizeof(_data[0]) * 8)] &= ~(1 << (_hwAddress % (sizeof(_data[0]) * 8)));
	_data[_extendedAddress / (sizeof(_data[0]) * 8)] |= (1 << (_extendedAddress % (sizeof(_data[0]) * 8)));
}

inline void TriStateOutput::moveDown() {
	_data[_hwAddress / (sizeof(_data[0]) * 8)] |= (1 << (_hwAddress % (sizeof(_data[0]) * 8)));
	_data[_extendedAddress / (sizeof(_data[0]) * 8)] &= ~(1 << (_extendedAddress % (sizeof(_data[0]) * 8)));
}


inline void TriStateOutput::evaluate() {
	switch (_state) {
	case ActiveUp: {
		evaluateBranchesActive<true>();
		return;
	}
	case ActiveDown: {
		evaluateBranchesActive<false>();
		return;
	}
	case PassiveUp: {
		evaluateBranchesPassive<true>();
		return;
	}
	case PassiveDown: {
		evaluateBranchesPassive<false>();
		return;
	}
	case TimeoutUp: {
		evaluateBranchesTimeout<true>();
		return;
	}
	case TimeoutDown: {
		evaluateBranchesTimeout<false>();
		return;
	}
	case StoppedUp: {
		evaluateBranchesStopped<true>();
		return;
	}
	case StoppedDown: {
		evaluateBranchesStopped<false>();
		return;
	}
	default: {
		UARTprintf("Unsupported switch case in TriStateOutput::evaluate()\n");
		break;
	}
	}
}

template<bool up> inline void TriStateOutput::evaluateBranchesActive() {
	_timer--;
	if(0 == _timer) {
		_state = up ? PassiveUp : PassiveDown;
		stop();
	}

	const std::vector<input::Input*>& inputs = input::InputManagerFactory::get()->inputs();
	for(u32 index = 0; index < (up ? _inputsDown.size() : _inputsUp.size()); index++) {
		if(input::NoChangeEvent != inputs[(up ? _inputsDown : _inputsUp)[index]]->changed()) {
			_state = up ? StoppedUp : StoppedDown;
			stop();
			return;
		}
	}

	for(u32 index = 0; index < _inputs.size(); index++) {
		if(input::NoChangeEvent != inputs[_inputs[index]]->changed()) {
			_state = up ? StoppedUp : StoppedDown;
			stop();
			return;
		}
	}
}

template<bool up> inline void TriStateOutput::evaluateBranchesPassive() {
	const std::vector<input::Input*>& inputs = input::InputManagerFactory::get()->inputs();
	for(u32 index = 0; index < (up ? _inputsDown.size() : _inputsUp.size()); index++) {
		switch (inputs[(up ? _inputsDown : _inputsUp)[index]]->changed()) {
		case input::ChangeEvent: {
			_state = up ? ActiveDown : ActiveUp;
			_timer = _timeoutOFF;
			up ? moveDown() : moveUp();
			return;
		}
		case input::DeferredChangeEvent: {
			if(0 != _timeoutON) {
				_timer = _timeoutON;
				_state = up ? TimeoutDown : TimeoutUp;
				return;
			}

			break;
		}
		default:
			break;
		}
	}

	for(u32 index = 0; index < _inputs.size(); index++) {
		switch (inputs[_inputs[index]]->changed()) {
		case input::ChangeEvent: {
			_state = up ? ActiveDown : ActiveUp;
			up ? moveDown() : moveUp();
			_timer = _timeoutOFF;
			return;
		}
		case input::DeferredChangeEvent: {
			if(0 != _timeoutON) {
				_timer = _timeoutON;
				_state = up ? TimeoutDown : TimeoutUp;
				return;
			}

			break;
		}
		default:
			break;
		}
	}
}

template<bool up> inline void TriStateOutput::evaluateBranchesTimeout() {
	const std::vector<input::Input*>& inputs = input::InputManagerFactory::get()->inputs();
	for(u32 index = 0; index < (up ? _inputsDown.size() : _inputsUp.size()); index++) {
		if(input::NoChangeEvent != inputs[(up ? _inputsDown : _inputsUp)[index]]->changed()) {
			_state = up ? PassiveDown : PassiveUp;
			stop();
			return;
		}
	}

	for(u32 index = 0; index < (up ? _inputsUp.size() : _inputsDown.size()); index++) {
		if(input::ChangeEvent == inputs[(up ? _inputsUp : _inputsDown)[index]]->changed()) {
			_timer = _timeoutOFF;
			_state = up ? ActiveUp : ActiveDown;
			up ? moveUp() : moveDown();
			return;
		}
	}

	for(u32 index = 0; index < _inputs.size(); index++) {
		switch (inputs[_inputs[index]]->changed()) {
		case input::ChangeEvent: {
			_timer = _timeoutOFF;
			_state = up ? ActiveUp : ActiveDown;
			up ? moveUp() : moveDown();
			return;
		}
		case input::DeferredChangeEvent: {
			_state = up ? PassiveDown : PassiveUp;
			return;
		}
		default:
			break;
		}
	}

	_timer--;
	if(0 == _timer) {
		_state = up ? ActiveUp : ActiveDown;
		_timer = _timeoutOFF;
		up ? moveUp() : moveDown();
	}
}

template<bool up> inline void TriStateOutput::evaluateBranchesStopped() {
	const std::vector<input::Input*>& inputs = input::InputManagerFactory::get()->inputs();
	for(u32 index = 0; index < (up ? _inputsUp.size() : _inputsDown.size()); index++) {
		if(input::NoChangeEvent != inputs[(up ? _inputsUp : _inputsDown)[index]]->changed()) {
			_state = up ? ActiveUp : ActiveDown;
			up ? moveUp() : moveDown();
			return;
		}
	}

	for(u32 index = 0; index < _inputs.size(); index++) {
		switch (inputs[_inputs[index]]->changed()) {
		case input::ChangeEvent: {
			_state = up ? ActiveDown : ActiveUp;
			up ? moveDown() : moveUp();
			return;
		}
		case input::DeferredChangeEvent: {
			if(0 != _timeoutON) {
				_timer = _timeoutON;
				_state = up ? TimeoutDown : TimeoutUp;
				return;
			}

			break;
		}
		default:
			break;
		}
	}

	for(u32 index = 0; index < (up ? _inputsDown.size() : _inputsUp.size()); index++) {
		switch (inputs[(up ? _inputsDown : _inputsUp)[index]]->changed()) {
		case input::ChangeEvent: {
			_state = up ? ActiveDown : ActiveUp;
			up ? moveDown() : moveUp();
			return;
		}
		case input::DeferredChangeEvent: {
			if(0 != _timeoutON) {
				_timer = _timeoutON;
				_state = up ? TimeoutDown : TimeoutUp;
				return;
			}

			break;
		}
		default:
			break;
		}
	}
}

} // output
} // manager

#endif // _TRISTATEOUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: tristateoutput.hpp =====================
