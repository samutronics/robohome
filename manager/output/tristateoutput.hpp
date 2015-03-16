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
				_inputsDown(inputsDown) {}

inline void TriStateOutput::evaluate() {
}

} // output
} // manager

#endif // _TRISTATEOUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: tristateoutput.hpp =====================
