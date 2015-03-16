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
public: inline Input(const project::InputType type, cu16 hwAddress, const std::vector<u32>& dataChanged, const std::vector<u32>& dataCurrent, const std::vector<u32>& dataPrevious);

public: inline StateChange changed() const;

private: const project::InputType	_type;
private: cu16						_hwAddress;
private: const std::vector<u32>&	_dataChanged;
private: const std::vector<u32>&	_dataCurrent;
private: const std::vector<u32>&	_dataPrevious;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline Input::Input(const project::InputType type, cu16 hwAddress, const std::vector<u32>& dataChanged, const std::vector<u32>& dataCurrent, const std::vector<u32>& dataPrevious):
		_type(type),
		_hwAddress(hwAddress),
		_dataChanged(dataChanged),
		_dataCurrent(dataCurrent),
		_dataPrevious(dataPrevious) {}

inline StateChange Input::changed() const {
	if(!(_dataChanged[_hwAddress / sizeof(_dataChanged[0])] & (1 << (_hwAddress % sizeof(_dataChanged[0]))))) { return NoChangeEvent;}

	if((_type == project::BothEdges) &&
			((_dataCurrent[_hwAddress / sizeof(_dataCurrent[0])] & (1 << (_hwAddress % sizeof(_dataCurrent[0])))) &&
					(!(_dataPrevious[_hwAddress / sizeof(_dataPrevious[0])] & (1 << (_hwAddress % sizeof(_dataPrevious[0]))))))) {
		return ChangeEvent;
	}
	else if((_type == project::RisingEdge) &&
			((_dataCurrent[_hwAddress / sizeof(_dataCurrent[0])] & (1 << (_hwAddress % sizeof(_dataCurrent[0])))) &&
					!(_dataPrevious[_hwAddress / sizeof(_dataPrevious[0])] & (1 << (_hwAddress % sizeof(_dataPrevious[0])))))) {
		return ChangeEvent;
	}
	else if((_type == project::DeferredBothEdges) &&
			((_dataCurrent[_hwAddress / sizeof(_dataCurrent[0])] & (1 << (_hwAddress % sizeof(_dataCurrent[0])))) &&
					(!(_dataPrevious[_hwAddress / sizeof(_dataPrevious[0])] & (1 << (_hwAddress % sizeof(_dataPrevious[0]))))))) {
		return DeferredChangeEvent;
	}
	else if((_type == project::DeferredRisingEdge) &&
			((_dataCurrent[_hwAddress / sizeof(_dataCurrent[0])] & (1 << (_hwAddress % sizeof(_dataCurrent[0])))) &&
					!(_dataPrevious[_hwAddress / sizeof(_dataPrevious[0])] & (1 << (_hwAddress % sizeof(_dataPrevious[0])))))) {
		return DeferredChangeEvent;
	}

	return NoChangeEvent;
}

} // input
} // manager

#endif // _INPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ============================ end of file: input.hpp =========================
