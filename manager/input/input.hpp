// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#include "metainput.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace input {

enum StateChange {
	NoChangeEvent,
	DeferredChangeEvent,
	ChangeEvent
};

class Input {
public: inline Input(const project::InputType type, cu16 hwAddress, const std::vector<u32>& dataCurrent, const std::vector<u32>& dataPrevious);

public: inline StateChange changed() const;

private: const project::InputType	_type;
private: cu16						_hwAddress;
private: const std::vector<u32>&	_dataCurrent;
private: const std::vector<u32>&	_dataPrevious;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline Input::Input(const project::InputType type, cu16 hwAddress, const std::vector<u32>& dataCurrent, const std::vector<u32>& dataPrevious):
				_type(type),
				_hwAddress(hwAddress),
				_dataCurrent(dataCurrent),
				_dataPrevious(dataPrevious) {}

inline StateChange Input::changed() const {
	cu32 bitCount = (sizeof(_dataCurrent[0]) * 8);

	switch(_type) {
	case project::BothEdges: {
		return ((_dataCurrent[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount))) ^
				(_dataPrevious[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount)))) ? ChangeEvent : NoChangeEvent;
	}
	case project::RisingEdge: {
		return ((_dataCurrent[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount))) &&
				!(_dataPrevious[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount)))) ? ChangeEvent : NoChangeEvent;
	}
	case project::DeferredBothEdges: {
		return ((_dataCurrent[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount))) ^
				(_dataPrevious[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount)))) ? DeferredChangeEvent : NoChangeEvent;
	}
	case project::DeferredRisingEdge: {
		return ((_dataCurrent[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount))) &&
				!(_dataPrevious[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount)))) ? DeferredChangeEvent : NoChangeEvent;
	}
	default: {
		UARTprintf("Unsupported input type\n");
		return NoChangeEvent;
	}
	}
}

} // input
} // manager

#endif // _INPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ============================ end of file: input.hpp =========================
