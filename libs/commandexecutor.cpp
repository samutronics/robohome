// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.03.2015.
//! \note
// =============================================================================
#include "inputmanager.hpp"
#include "commandexecutor.hpp"

using namespace libs;
using namespace systemGlobal;
using namespace manager::input;
using namespace service::inbound::configuration;

bool CommandExecutor::executeRead(const CommandsIterator& it) {
	switch(it.key() & ComponentIDMask) {
	case ComponentIDOutput: {

	}
	case ComponentIDProject: {

	}
	case ComponentIDIrrigation: {

	}
	default: {
		UARTprintf("Unsupported component ID\n");
	}
	}

	return false;
}

bool CommandExecutor::executeWrite(const CommandsIterator& it) {
	switch(it.key() & ComponentIDMask) {
	case ComponentIDInput: {
		InputManagerFactory::get()->write(it.key() & addressMask, strtoul(it.value().c_str(), NULL, 10));
		return true;
	}
	case ComponentIDOutput: {

	}
	case ComponentIDProject: {

	}
	case ComponentIDIrrigation: {

	}
	default: {
		UARTprintf("Unsupported component ID\n");
	}
	}

	return false;
}

// =============================================================================
//! \file
//! \copyright
// ====================== end of file: commandexecutor.cpp =====================
