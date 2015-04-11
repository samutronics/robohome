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
#include "devicestatistic.hpp"
#include "devicerequestFactory.hpp"

using namespace std;
using namespace libs;
using namespace systemGlobal;
using namespace manager::project;
using namespace service::exosite;
using namespace service::outbound::configuration;
using namespace service::irrigation::configuration;

deviceRequestFactory::deviceRequestFactory():
		writeRequestOutbound(255, 0),
		readRequestOutbound(255, 0),
		_inputs(ProjectManagerFactory::get()->input().count()) {
	for(u32 index = 0; index < _inputs.size(); ++index) {
		s8 buf[11];
		sprintf(buf, "%X", cmdWrite | ComponentIDInputService | index);
		_inputs[index] = string(buf);
	}
}

bool deviceRequestFactory::updateEntryByResponse(statisticEntry& entry, const std::string& response) {
	//
	// Find the desired alias in the buffer.
	//
	u32 pcValueStart = response.find(entry.entryAliasInCloud);

	//
	// If we couldn't find it, return a zero. Otherwise, continue extracting
	// the value.
	//
	if(string::npos == pcValueStart) {return false;}

	//
	// Find the equals-sign, which should be just before the start of the
	// value.
	//
	pcValueStart = response.find('=', pcValueStart);

	if(string::npos == pcValueStart) {return false;}

	//
	// Advance to the first character of the value.
	//
	pcValueStart++;

	u32 pcValueEnd = response.find('&', pcValueStart);

	if(pcValueStart == pcValueEnd) {
		entry.setValue("");
		return true;
	}

	if(string::npos == pcValueEnd) {pcValueEnd = response.length();}

	entry.setValue(response.substr(pcValueStart, pcValueEnd - pcValueStart));
	return true;
}

const std::string& deviceRequestFactory::readRequest() {
	readRequestOutbound.clear();
	for(u32 index = 0; index < _inputs.size(); ++index) {
		readRequestOutbound += _inputs[index];
		readRequestOutbound += "=&";
	}

	readRequestOutbound.erase(readRequestOutbound.size() - 1);
    return readRequestOutbound;
}

const std::string& deviceRequestFactory::writeRequest() {
	writeRequestOutbound.clear();

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
	MediatorFactory::get()->execute(cmdIt, writeRequestOutbound);
	if('&' != writeRequestOutbound[writeRequestOutbound.size() - 1]) {
		writeRequestOutbound += '&';
	}

	cmdIt.next();
	MediatorFactory::get()->execute(cmdIt, writeRequestOutbound);
	if('&' != writeRequestOutbound[writeRequestOutbound.size() - 1]) {
		writeRequestOutbound += '&';
	}

	cmdIt.next();
	MediatorFactory::get()->execute(cmdIt, writeRequestOutbound);
	return writeRequestOutbound;
}

// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.cpp ==================
