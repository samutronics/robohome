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
#include "../projectconfiguration.hpp"

namespace manager {
namespace output {

class Output {
public: inline Output(const u16 hwAddress, const u16 timeout, const std::vector<u16>& inputs, std::vector<u32>& data);
public: inline Output& operator=(const Output& other);
public: inline virtual void evaluate();

protected: u16 _deferredTime;
protected: u16 _currentTime;
protected: std::vector<u32>& _data;
protected: const u16 _hwAddress;
protected: const u16 _timeout;
protected: const std::vector<u16>& _inputs;
};

inline Output::Output(const u16 hwAddress, const u16 timeout, const std::vector<u16>& inputs, std::vector<u32>& data):
		_data(data),
		_hwAddress(hwAddress),
		_timeout(timeout),
		_inputs(inputs) {}

inline Output& Output::operator=(const Output& other) {
	Output(other._hwAddress, other._timeout, other._inputs, other._data);
	return *this;
}

inline void Output::evaluate() {

}

} // output
} // manager

#endif // _OUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// =========================== end of file: output.hpp =========================
