// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#include "inputmanager.hpp"

using namespace manager::input;

InputManager* InputManager::_instance;

InputManager::~InputManager() {
	for(u32 index = 0; index < _inputs.size(); index++) {delete _inputs[index];}

	for(u32 index = 0; index < _lock.size(); index++) {vSemaphoreDelete(_lock[index]);}
}

// =============================================================================
//! \file
//! \copyright
// ======================== end of file: inputmanager.cpp ======================
