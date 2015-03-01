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
	stateUnchanged,
	stateChangedUp,
	stateChangedDown
};

class Input {
public: inline Input(const project::TriggerType type, const u16 hwAddress, const std::vector<u32>& dataChanged, const std::vector<u32>& dataCurrent, const std::vector<u32>& dataPrevious);
public: inline Input& operator=(const Input& other);

public: inline StateChange changed(u16& deferTime) const;

private: const project::TriggerType	_type;
private: const u16					_hwAddress;
private: const std::vector<u32>&	_dataChanged;
private: const std::vector<u32>&	_dataCurrent;
private: const std::vector<u32>&	_dataPrevious;
};

inline Input::Input(const project::TriggerType type, const u16 hwAddress, const std::vector<u32>& dataChanged, const std::vector<u32>& dataCurrent, const std::vector<u32>& dataPrevious):
		_type(type),
		_hwAddress(hwAddress),
		_dataChanged(dataChanged),
		_dataCurrent(dataCurrent),
		_dataPrevious(dataPrevious) {}

inline Input& Input::operator=(const Input& other) {
	Input(other._type, other._hwAddress, other._dataChanged, other._dataCurrent, other._dataPrevious);
	return *this;
}

inline StateChange Input::changed(u16& deferTime) const {
	deferTime = 0;
	if(!(_dataChanged[_hwAddress / sizeof(_dataChanged[0])] & (1 << (_hwAddress % sizeof(_dataChanged[0]))))) { return stateUnchanged;}

	if((_type == project::triggerBothEdges) &&
			((_dataCurrent[_hwAddress / sizeof(_dataCurrent[0])] & (1 << (_hwAddress % sizeof(_dataCurrent[0])))) &&
					(!(_dataPrevious[_hwAddress / sizeof(_dataPrevious[0])] & (1 << (_hwAddress % sizeof(_dataPrevious[0]))))))) {
		return stateChangedUp;
	}
	else if((_type == project::triggerBothEdges) &&
			((!(_dataCurrent[_hwAddress / sizeof(_dataCurrent[0])] & (1 << (_hwAddress % sizeof(_dataCurrent[0]))))) &&
					(_dataPrevious[_hwAddress / sizeof(_dataPrevious[0])] & (1 << (_hwAddress % sizeof(_dataPrevious[0])))))) {
		return stateChangedDown;
	}
	else if((_type == project::triggerRisingEdge) &&
			((_dataCurrent[_hwAddress / sizeof(_dataCurrent[0])] & (1 << (_hwAddress % sizeof(_dataCurrent[0])))) &&
					!(_dataPrevious[_hwAddress / sizeof(_dataPrevious[0])] & (1 << (_hwAddress % sizeof(_dataPrevious[0])))))) {
		return stateChangedUp;
	}

	return stateUnchanged;
}

} // input
} // manager

#endif // _INPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ============================ end of file: input.hpp =========================
