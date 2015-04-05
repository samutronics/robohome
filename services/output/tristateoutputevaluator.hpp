// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#ifndef _TRISTATEOUTPUTEVALUATOR_HPP_
#define _TRISTATEOUTPUTEVALUATOR_HPP_

#include "outputevaluator.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace outbound {

class TriStateOutputEvaluator: public OutputEvaluator {
public: inline TriStateOutputEvaluator(cu16 hwAddress, cu16 timeoutON, cu16 timeoutOFF, const std::vector<u16>& inputsUpDown, std::vector<u32>& data, cu16 extendedAddress, const std::vector<u16>& inputsUp, const std::vector<u16>& inputsDown);

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

inline TriStateOutputEvaluator::TriStateOutputEvaluator(cu16 hwAddress, cu16 timeoutON, cu16 timeoutOFF, const std::vector<u16>& inputsUpDown, std::vector<u32>& data, cu16 extendedAddress, const std::vector<u16>& inputsUp, const std::vector<u16>& inputsDown):
						OutputEvaluator(hwAddress, timeoutON, timeoutOFF, inputsUpDown, data),
						_extendedAddress(extendedAddress),
						_inputsUp(inputsUp),
						_inputsDown(inputsDown) {
	_state = PassiveUp;
}

inline void TriStateOutputEvaluator::stop() {
	_data[_hwAddress / (sizeof(_data[0]) * 8)] &= ~(1 << (_hwAddress % (sizeof(_data[0]) * 8)));
	_data[_extendedAddress / (sizeof(_data[0]) * 8)] &= ~(1 << (_extendedAddress % (sizeof(_data[0]) * 8)));
}

inline void TriStateOutputEvaluator::moveUp() {
	_data[_hwAddress / (sizeof(_data[0]) * 8)] &= ~(1 << (_hwAddress % (sizeof(_data[0]) * 8)));
	_data[_extendedAddress / (sizeof(_data[0]) * 8)] |= (1 << (_extendedAddress % (sizeof(_data[0]) * 8)));
}

inline void TriStateOutputEvaluator::moveDown() {
	_data[_hwAddress / (sizeof(_data[0]) * 8)] |= (1 << (_hwAddress % (sizeof(_data[0]) * 8)));
	_data[_extendedAddress / (sizeof(_data[0]) * 8)] &= ~(1 << (_extendedAddress % (sizeof(_data[0]) * 8)));
}


inline void TriStateOutputEvaluator::evaluate() {
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

template<bool up> inline void TriStateOutputEvaluator::evaluateBranchesActive() {
	_timer--;
	if(0 == _timer) {
		_state = up ? PassiveUp : PassiveDown;
		stop();
	}

	const std::vector<service::inbound::InputEvaluator*>& inputs = service::inbound::InputTaskFactory::get()->inputs();
	for(u32 index = 0; index < (up ? _inputsDown.size() : _inputsUp.size()); index++) {
		if(service::inbound::NoChangeEvent != inputs[(up ? _inputsDown : _inputsUp)[index]]->changed()) {
			_state = up ? StoppedUp : StoppedDown;
			stop();
			return;
		}
	}

	for(u32 index = 0; index < _inputs.size(); index++) {
		if(service::inbound::NoChangeEvent != inputs[_inputs[index]]->changed()) {
			_state = up ? StoppedUp : StoppedDown;
			stop();
			return;
		}
	}
}

template<bool up> inline void TriStateOutputEvaluator::evaluateBranchesPassive() {
	const std::vector<service::inbound::InputEvaluator*>& inputs = service::inbound::InputTaskFactory::get()->inputs();
	for(u32 index = 0; index < (up ? _inputsDown.size() : _inputsUp.size()); index++) {
		switch (inputs[(up ? _inputsDown : _inputsUp)[index]]->changed()) {
		case service::inbound::ChangeEvent: {
			_state = up ? ActiveDown : ActiveUp;
			_timer = _timeoutOFF;
			up ? moveDown() : moveUp();
			return;
		}
		case service::inbound::DeferredChangeEvent: {
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
		case service::inbound::ChangeEvent: {
			_state = up ? ActiveDown : ActiveUp;
			up ? moveDown() : moveUp();
			_timer = _timeoutOFF;
			return;
		}
		case service::inbound::DeferredChangeEvent: {
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

template<bool up> inline void TriStateOutputEvaluator::evaluateBranchesTimeout() {
	const std::vector<service::inbound::InputEvaluator*>& inputs = service::inbound::InputTaskFactory::get()->inputs();
	for(u32 index = 0; index < (up ? _inputsDown.size() : _inputsUp.size()); index++) {
		if(service::inbound::NoChangeEvent != inputs[(up ? _inputsDown : _inputsUp)[index]]->changed()) {
			_state = up ? PassiveDown : PassiveUp;
			stop();
			return;
		}
	}

	for(u32 index = 0; index < (up ? _inputsUp.size() : _inputsDown.size()); index++) {
		if(service::inbound::ChangeEvent == inputs[(up ? _inputsUp : _inputsDown)[index]]->changed()) {
			_timer = _timeoutOFF;
			_state = up ? ActiveUp : ActiveDown;
			up ? moveUp() : moveDown();
			return;
		}
	}

	for(u32 index = 0; index < _inputs.size(); index++) {
		switch (inputs[_inputs[index]]->changed()) {
		case service::inbound::ChangeEvent: {
			_timer = _timeoutOFF;
			_state = up ? ActiveUp : ActiveDown;
			up ? moveUp() : moveDown();
			return;
		}
		case service::inbound::DeferredChangeEvent: {
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

template<bool up> inline void TriStateOutputEvaluator::evaluateBranchesStopped() {
	const std::vector<service::inbound::InputEvaluator*>& inputs = service::inbound::InputTaskFactory::get()->inputs();
	for(u32 index = 0; index < (up ? _inputsUp.size() : _inputsDown.size()); index++) {
		if(service::inbound::NoChangeEvent != inputs[(up ? _inputsUp : _inputsDown)[index]]->changed()) {
			_state = up ? ActiveUp : ActiveDown;
			up ? moveUp() : moveDown();
			return;
		}
	}

	for(u32 index = 0; index < _inputs.size(); index++) {
		switch (inputs[_inputs[index]]->changed()) {
		case service::inbound::ChangeEvent: {
			_state = up ? ActiveDown : ActiveUp;
			up ? moveDown() : moveUp();
			return;
		}
		case service::inbound::DeferredChangeEvent: {
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
		case service::inbound::ChangeEvent: {
			_state = up ? ActiveDown : ActiveUp;
			up ? moveDown() : moveUp();
			return;
		}
		case service::inbound::DeferredChangeEvent: {
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

} // outbound
} // service

#endif // _TRISTATEOUTPUTEVALUATOR_HPP_
// =============================================================================
//! \file
//! \copyright
// ================= end of file: tristateoutputevaluator.hpp ==================
