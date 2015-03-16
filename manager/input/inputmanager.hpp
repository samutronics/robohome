// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#ifndef _INPUTMANAGER_HPP_
#define _INPUTMANAGER_HPP_

#include "input.hpp"
#include "projectmanager.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace input {

class InputManager {
public: static inline InputManager* getInstance();

public: virtual ~InputManager();
public: inline const std::vector<Input*>& inputs() const;
public: inline void reset();
public: inline void write(cu16 address, cu8 data);
public: inline void write(const std::vector<u32>& data);

private: inline InputManager();

private: std::vector<Input*> _inputs;
private: std::vector<u32> _dataChanged;
private: std::vector<u32> _dataCurrent;
private: std::vector<u32> _dataPrevious;
private: std::vector<xSemaphoreHandle> _lock;

private: static InputManager* _instance;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline InputManager* InputManager::getInstance() {
	if(!_instance) {
		xSemaphoreHandle sync = NULL;
		if(!sync) {
			sync = xSemaphoreCreateMutex();
		}

		 xSemaphoreTake(sync, 0);
		if(!_instance) {
			_instance = new InputManager();
		}

		xSemaphoreGive(sync);
	}

	return _instance;
}

inline InputManager::InputManager():
				_inputs(project::ProjectManager::getInstance()->input().count()),
				_dataChanged((_inputs.size() / (sizeof(_dataChanged[0]) * 8)) + ((_inputs.size() % (sizeof(_dataChanged[0]) * 8)) ? 1 : 0), 0),
				_dataCurrent(_dataChanged.size(), 0),
				_dataPrevious(_dataChanged.size(), 0),
				_lock(_dataChanged.size()) {
	project::metaInput data = project::ProjectManager::getInstance()->input();
	for(u32 index = 0; index < data.count(); index++) {
		_inputs[index] = new Input(data.trigger(), index, _dataChanged, _dataCurrent, _dataPrevious);
		data.next();
	}

	for(u32 index = 0; index < _dataChanged.size(); index++) {_lock[index] = xSemaphoreCreateMutex();}
}

inline const std::vector<Input*>& InputManager::inputs() const {return _inputs;}

inline void InputManager::reset() {
	for(u32 index = 0; index < _dataChanged.size(); index++) {_dataChanged[index] = 0;}
}

inline void InputManager::write(cu16 address, cu8 data) {
	if(address < _dataChanged.size() * sizeof(_dataChanged[0]) * 8) {return;}

	xSemaphoreTake(_lock[address / sizeof(_dataChanged[0])], 0);
	if(!(_dataChanged[address / sizeof(_dataChanged[0])] & (1 << address % sizeof(_dataChanged[0])))) {
		_dataPrevious[address / sizeof(_dataChanged[0])] = ((_dataPrevious[address / sizeof(_dataChanged[0])] & ~(1 << address % sizeof(_dataChanged[0]))) | (_dataCurrent[address / sizeof(_dataChanged[0])] & (1 << address % sizeof(_dataChanged[0]))));
		_dataCurrent[address / sizeof(_dataChanged[0])] = ((_dataCurrent[address / sizeof(_dataChanged[0])] & ~(1 << address % sizeof(_dataChanged[0]))) | ((data ? 1 : 0) << (address % sizeof(_dataChanged[0]))));
		_dataChanged[address / sizeof(_dataChanged[0])] |= (1 << address % sizeof(_dataChanged[0]));
	}

	xSemaphoreGive(_lock[address / sizeof(_dataChanged[0])]);
}

inline void InputManager::write(const std::vector<u32>& data) {
	if(data.size() != _dataChanged.size()) {return;}

	for(u32 dwordIndex = 0; dwordIndex < _dataChanged.size(); dwordIndex++) {
		xSemaphoreTake(_lock[dwordIndex], 0);
		for(u32 bitIndex = 0; bitIndex < sizeof(_dataChanged[0]) * 8; bitIndex++) {
			if(!(_dataChanged[dwordIndex] & (1 << bitIndex))) {
				_dataPrevious[dwordIndex] = ((_dataPrevious[dwordIndex] & ~(1 << bitIndex)) | (_dataCurrent[dwordIndex] & (1 << bitIndex)));
				_dataCurrent[dwordIndex] = ((_dataCurrent[dwordIndex] & ~(1 << bitIndex)) | (data[dwordIndex] & (1 << bitIndex)));
				_dataChanged[dwordIndex] |= (1 << bitIndex);
			}
		}

		xSemaphoreGive(_lock[dwordIndex]);
	}
}

} // input
} // manager

#endif // _INPUTMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: inputmanager.hpp ======================
