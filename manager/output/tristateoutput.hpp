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
public: inline TriStateOutput(cu16 hwAddress, cu16 timeout, const std::vector<u16>& inputsUpDown, std::vector<u32>& data, cu16 extendedAddress, const std::vector<u16>& inputsUp, const std::vector<u16>& inputsDown);
public: inline TriStateOutput& operator=(const TriStateOutput& other);
public: inline virtual void evaluate();

private: cu16						_extendedAddress;
private: const std::vector<u16>&	_inputsUp;
private: const std::vector<u16>&	_inputsDown;
};

inline TriStateOutput::TriStateOutput(cu16 hwAddress, cu16 timeout, const std::vector<u16>& inputsUpDown, std::vector<u32>& data, cu16 extendedAddress, const std::vector<u16>& inputsUp, const std::vector<u16>& inputsDown):
				Output(hwAddress, timeout, inputsUpDown, data),
				_extendedAddress(extendedAddress),
				_inputsUp(inputsUp),
				_inputsDown(inputsDown) {}

inline TriStateOutput& TriStateOutput::operator=(const TriStateOutput& other) {
	TriStateOutput(other._hwAddress, other._timeout, other._inputs, other._data, other._extendedAddress, other._inputsUp, other._inputsDown);
	return *this;
}

inline void TriStateOutput::evaluate() {

}

} // output
} // manager

#endif // _TRISTATEOUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: tristateoutput.hpp =====================
