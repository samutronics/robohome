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
	bool userInteractionOccured = false;
	const std::vector<input::Input*>& input = input::InputManager::getInstance()->inputs();
	for(u32 index = 0; index < _inputs.size(); index++) {
		if(input[_inputs[index]]->changed()) {
			writeBit(_hwAddress);
			userInteractionOccured = true;
			break;
		}
	}

}

} // output
} // manager

#endif // _OUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// =========================== end of file: output.hpp =========================
