// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#include "outputmanager.hpp"

using namespace std;
using namespace libs;
using namespace manager::output;
using namespace service::outbound::configuration;

OutputManager::~OutputManager() {
	for(std::map<u32, Output*>::iterator it = _output.begin(); it != _output.end(); ++it) {delete it->second;}
}

bool OutputManager::write(const CommandsIterator& it) {
	return false;
}

bool OutputManager::read(const CommandsIterator& it, std::string& result) const {
	switch (it.key() & CmdOutputMask) {
	case CmdReadOutput: {
		for(u32 index = 0; index < _data.size(); index++) {
			s8 buf[10];
			sprintf(buf, "%d", _data[index]);
			result += buf;
		}

		return true;
	}
	case CmdReadOutputTime: {
		map<u32, Output*>::const_iterator o = _output.find(it.key() & addressMask);
		if(!o->second) {
			return false;
		}

		s8 buf[10];
		sprintf(buf, "%d", o->second->time());
		result += buf;
		return true;
	}
	default: {
		UARTprintf("Unsupported command OutputManager::read()\n");
		return false;
	}
	}
}

// =============================================================================
//! \file
//! \copyright
// ======================= end of file: outputmanager.cpp ======================
