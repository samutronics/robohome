// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#ifndef _INPUTEVALUATOR_HPP_
#define _INPUTEVALUATOR_HPP_

#include "metainput.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace inbound {

enum StateChange {
	NoChangeEvent,
	DeferredChangeEvent,
	ChangeEvent
};

class InputEvaluator {
public: inline InputEvaluator(const manager::project::InputType type, cu16 hwAddress, const std::vector<u32>& dataChnaged, const std::vector<u32>& dataCurrent, const std::vector<u32>& dataPrevious);

public: inline StateChange changed() const;

private: const manager::project::InputType	_type;
private: cu16								_hwAddress;
private: const std::vector<u32>&			_dataChanged;
private: const std::vector<u32>&			_dataCurrent;
private: const std::vector<u32>&			_dataPrevious;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline InputEvaluator::InputEvaluator(const manager::project::InputType type, cu16 hwAddress, const std::vector<u32>& dataChnaged, const std::vector<u32>& dataCurrent, const std::vector<u32>& dataPrevious):
				_type(type),
				_hwAddress(hwAddress),
				_dataChanged(dataChnaged),
				_dataCurrent(dataCurrent),
				_dataPrevious(dataPrevious) {}

inline StateChange InputEvaluator::changed() const {
	cu32 bitCount = (sizeof(_dataCurrent[0]) * 8);

	if(!(_dataChanged[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount)))) { return NoChangeEvent;}

	switch(_type) {
	case manager::project::BothEdges: {
		return ((_dataCurrent[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount))) ^
				(_dataPrevious[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount)))) ? ChangeEvent : NoChangeEvent;
	}
	case manager::project::RisingEdge: {
		return ((_dataCurrent[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount))) &&
				!(_dataPrevious[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount)))) ? ChangeEvent : NoChangeEvent;
	}
	case manager::project::DeferredBothEdges: {
		return ((_dataCurrent[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount))) ^
				(_dataPrevious[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount)))) ? DeferredChangeEvent : NoChangeEvent;
	}
	case manager::project::DeferredRisingEdge: {
		return ((_dataCurrent[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount))) &&
				!(_dataPrevious[_hwAddress / bitCount] & (1 << (_hwAddress % bitCount)))) ? DeferredChangeEvent : NoChangeEvent;
	}
	default: {
		UARTprintf("Unsupported input type\n");
		return NoChangeEvent;
	}
	}
}

} // inbound
} // service

#endif // _INPUTEVALUATOR_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: inputevaluator.hpp =====================
