// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#include "mediator.hpp"
#include "outputmanager.hpp"

using namespace std;
using namespace libs;
using namespace systemGlobal;
using namespace manager::output;
using namespace service::outbound::configuration;

OutputManager::OutputManager(): _data(project::ProjectManagerFactory::get()->output().count() + project::ProjectManagerFactory::get()->triStateOutput().count()
		/ (sizeof(_data[0] * 8)) + ((project::ProjectManagerFactory::get()->output().count() + project::ProjectManagerFactory::get()->triStateOutput().count() % (sizeof(_data[0]) * 8)) ? 1 : 0), 0) {
	std::vector<u16> simpleTmp;
	project::metaOutput simpleoutput = project::ProjectManagerFactory::get()->output();
	for(u32 index = 0; index < simpleoutput.count(); index++) {
		simpleoutput.inputs(simpleTmp);
		_output[simpleoutput.address()] = new Output(simpleoutput.address(), simpleoutput.timeoutON(), simpleoutput.timeoutOFF(), simpleTmp, _data);
		simpleTmp.clear();
		simpleoutput.next();
	}

	std::vector<u16> tristateTmpUp;
	std::vector<u16> tristateTmpDown;
	project::metaTriStateOutput tristateutput = project::ProjectManagerFactory::get()->triStateOutput();
	for(u32 index = 0; index < tristateutput.count(); index++) {
		tristateutput.inputs(simpleTmp);
		tristateutput.inputsUp(tristateTmpUp);
		tristateutput.inputsDown(tristateTmpDown);
		_output[tristateutput.address()] = new TriStateOutput(
				tristateutput.address(),
				tristateutput.timeoutON(),
				tristateutput.timeoutOFF(),
				simpleTmp,
				_data,
				tristateutput.extendedAddress(),
				tristateTmpUp,
				tristateTmpDown);

		simpleTmp.clear();
		tristateTmpUp.clear();
		tristateTmpDown.clear();
		tristateutput.next();
	}

	MediatorFactory::get()->attach(ComponentIDOutput, this);
}

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
