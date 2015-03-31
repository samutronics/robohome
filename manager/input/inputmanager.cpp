// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#include "inputmanager.hpp"
#include "commandsiterator.hpp"

using namespace libs;
using namespace manager::input;
using namespace service::inbound::configuration;

InputManager::~InputManager() {
	for(u32 index = 0; index < _inputs.size(); index++) {delete _inputs[index];}

	for(u32 index = 0; index < _lock.size(); index++) {vSemaphoreDelete(_lock[index]);}
}

bool InputManager::write(const CommandsIterator& it) {
	write(it.key() & addressMask, strtoul(it.value().c_str(), NULL, 10));
	return true;
}

bool InputManager::read(const CommandsIterator& it, std::string& result) const {
	return false;
}

// =============================================================================
//! \file
//! \copyright
// ======================== end of file: inputmanager.cpp ======================
