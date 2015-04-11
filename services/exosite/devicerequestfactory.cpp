// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#include "exosite.hpp"
#include "mediator.hpp"
#include "projectmanager.hpp"
#include "devicerequestFactory.hpp"

using namespace std;
using namespace libs;
using namespace systemGlobal;
using namespace manager::project;
using namespace service::exosite;
using namespace service::outbound::configuration;
using namespace service::irrigation::configuration;

deviceRequestFactory::deviceRequestFactory():
		_writeRequestOutbound(255, 0),
		_readRequestOutbound(255, 0),
		_inputs(ProjectManagerFactory::get()->input().count()) {
	for(u32 index = 0; index < _inputs.size(); ++index) {
		s8 buf[11];
		sprintf(buf, "%X", cmdWrite | ComponentIDInputService | index);
		_inputs[index] = string(buf);
	}
}

const std::string& deviceRequestFactory::readRequest() {
	_readRequestOutbound.clear();
	for(u32 index = 0; index < _inputs.size(); ++index) {
		_readRequestOutbound += _inputs[index];
		_readRequestOutbound += "=&";
	}

	_readRequestOutbound.erase(_readRequestOutbound.size() - 1);
    return _readRequestOutbound;
}

const std::string& deviceRequestFactory::writeRequest() {
	_writeRequestOutbound.clear();

	s8 buf[11];
	sprintf(buf, "%X", cmdRead | ComponentIDOutputService | CmdMassReadOutput);
	string cmd(buf);
	cmd += "=0&";
	sprintf(buf, "%X", cmdRead | ComponentIDOutputService | CmdMassReadOutputTime);
	cmd += buf;
	cmd += "=0&";
	sprintf(buf, "%X", cmdRead | ComponentIDIrrigation | CmdMassTimer);
	cmd += buf;
	cmd += "=0";

	CommandsIterator cmdIt(cmd);
	cmdIt.next();
	MediatorFactory::get()->execute(cmdIt, _writeRequestOutbound);
	if('&' != _writeRequestOutbound[_writeRequestOutbound.size() - 1]) {
		_writeRequestOutbound += '&';
	}

	cmdIt.next();
	MediatorFactory::get()->execute(cmdIt, _writeRequestOutbound);
	if('&' != _writeRequestOutbound[_writeRequestOutbound.size() - 1]) {
		_writeRequestOutbound += '&';
	}

	cmdIt.next();
	MediatorFactory::get()->execute(cmdIt, _writeRequestOutbound);
	return _writeRequestOutbound;
}

// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.cpp ==================
