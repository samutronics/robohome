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
#include "../projectconfiguration.hpp"

namespace manager {
namespace output {

class TriStateOutput: public Output {
public: inline TriStateOutput(cu16 hwAddress, cu16 timeoutON, cu16 timeoutOFF, const std::vector<u16>& inputsUpDown, std::vector<u32>& data, cu16 extendedAddress, const std::vector<u16>& inputsUp, const std::vector<u16>& inputsDown);

public: inline virtual void evaluate();

private: inline void evaluateBranchActiveUp();
private: inline void evaluateBranchActiveDown();
private: inline void evaluateBranchPassiveUp();
private: inline void evaluateBranchPassiveDown();
private: inline void evaluateBranchTimeoutUp();
private: inline void evaluateBranchTimeoutDown();
private: inline void evaluateBranchStoppedUp();
private: inline void evaluateBranchStoppedDown();

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

inline void TriStateOutput::evaluate() {
	switch (_state) {
	case ActiveUp: {
		evaluateBranchActiveUp();
		return;
	}
	case ActiveDown: {
		evaluateBranchActiveDown();
		return;
	}
	case PassiveUp: {
		evaluateBranchPassiveUp();
		return;
	}
	case PassiveDown: {
		evaluateBranchPassiveDown();
		return;
	}
	case TimeoutUp: {
		evaluateBranchTimeoutUp();
		return;
	}
	case TimeoutDown: {
		evaluateBranchTimeoutDown();
		return;
	}
	case StoppedUp: {
		evaluateBranchStoppedUp();
		return;
	}
	case StoppedDown: {
		evaluateBranchStoppedDown();
		return;
	}
	default: {
		UARTprintf("Unsupported switch case in TriStateOutput::evaluate()\n");
		break;
	}
	}
}

inline void TriStateOutput::evaluateBranchActiveUp() {

}

inline void TriStateOutput::evaluateBranchActiveDown() {

}

inline void TriStateOutput::evaluateBranchPassiveUp() {

}

inline void TriStateOutput::evaluateBranchPassiveDown() {

}

inline void TriStateOutput::evaluateBranchTimeoutUp() {

}

inline void TriStateOutput::evaluateBranchTimeoutDown() {

}

inline void TriStateOutput::evaluateBranchStoppedUp() {

}

inline void TriStateOutput::evaluateBranchStoppedDown() {

}

} // output
} // manager

#endif // _TRISTATEOUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: tristateoutput.hpp =====================
