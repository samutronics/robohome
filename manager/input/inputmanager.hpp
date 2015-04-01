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
#include "iinform.hpp"
#include "projectmanager.hpp"
#include "singletonfactory.hpp"
#include "commandsiterator.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace input {

class InputManager: public libs::IInform {
public: virtual ~InputManager();
public: inline const std::vector<Input*>& inputs() const;
public: inline void reset();
public: inline void write(cu16 address, cu8 data);
public: inline void write(const std::vector<u8>& data);

public: virtual bool write(const libs::CommandsIterator& it);
public: virtual bool read(const libs::CommandsIterator& it, std::string& result) const;

protected: InputManager();

private: std::vector<Input*>			_inputs;
private: std::vector<u32>				_dataChanged;
private: std::vector<u32>				_dataCurrent;
private: std::vector<u32>				_dataPrevious;
private: std::vector<xSemaphoreHandle>	_lock;
};

typedef libs::SingletonFactory<InputManager>	InputManagerFactory;

// =============================================================================
// Inline method implementation
// =============================================================================

inline const std::vector<Input*>& InputManager::inputs() const {
	return _inputs;
}

inline void InputManager::reset() {
	for(u32 index = 0; index < _dataChanged.size(); index++) {
		xSemaphoreTake(_lock[index], portMAX_DELAY);
		_dataChanged[index] = 0;
		xSemaphoreGive(_lock[index]);
	}
}

inline void InputManager::write(cu16 address, cu8 data) {
	cu32 bitCount = (sizeof(_dataChanged[0]) * 8);
	if(address > _dataChanged.size() * bitCount) {return;}

	xSemaphoreTake(_lock[address / bitCount], portMAX_DELAY);
	if(!(_dataChanged[address / bitCount] & (1 << address % bitCount))) {
		_dataPrevious[address / bitCount] = ((_dataPrevious[address / bitCount] & ~(1 << address % bitCount)) | (_dataCurrent[address / bitCount] & (1 << address % bitCount)));
		_dataCurrent[address / bitCount] = ((_dataCurrent[address / bitCount] & ~(1 << address % bitCount)) | ((data ? 1 : 0) << (address % bitCount)));
		_dataChanged[address / bitCount] |= (1 << address % bitCount);
	}

	xSemaphoreGive(_lock[address / bitCount]);
}

inline void InputManager::write(const std::vector<u8>& data) {
	for(u32 index = 0; index < data.size(); index++) {
		cu32 word = index / sizeof(_dataChanged[0]);
		xSemaphoreTake(_lock[word], portMAX_DELAY);

		cu32 byteMask = 0xFF << (index % sizeof(_dataChanged[0]) * 8);
		cu32 changeMask = ((~_dataChanged[word]) & byteMask);
		_dataPrevious[word] &= ~changeMask;
		_dataPrevious[word] |= (_dataCurrent[word] & changeMask);
		_dataCurrent[word] &= ~changeMask;
		_dataCurrent[word] |= ((data[index] << (index % sizeof(_dataCurrent[0]) * 8)) & changeMask);
		_dataChanged[word] |= byteMask;

		xSemaphoreGive(_lock[word]);
	}
}

} // input
} // manager

#endif // _INPUTMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: inputmanager.hpp ======================
