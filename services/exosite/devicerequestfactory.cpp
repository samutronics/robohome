// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#include "exosite.hpp"
#include "devicestatistic.hpp"
#include "devicerequestFactory.hpp"

using namespace std;
using namespace service::exosite;

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
    deviceStatistic::reset();
    while(deviceStatistic::next()) {
    	if(!deviceStatistic::current()->entryAliasInCloud) {continue;}

    	if((deviceStatistic::current()->access == READ_ONLY) || (deviceStatistic::current()->access == READ_WRITE)) {
    		addRequest(deviceStatistic::current()->entryAliasInCloud, readRequestOutbound);
    	}
    }

    return readRequestOutbound;
}

const std::string& deviceRequestFactory::writeRequest() {
	writeRequestOutbound.clear();
    deviceStatistic::reset();
    while(deviceStatistic::next()) {
    	if(!deviceStatistic::current()->entryAliasInCloud) {continue;}

    	if((deviceStatistic::current()->access == WRITE_ONLY) || (deviceStatistic::current()->access == READ_WRITE)) {
    		string str;
    		str.reserve(100);
    		deviceStatistic::current()->requestFormat(str);
    		addRequest(str, readRequestOutbound);
    	}
    }

    return writeRequestOutbound;
}

bool deviceRequestFactory::addRequest(const std::string& newRequest, std::string& buf) {
	if(!buf.empty()) {
		//
		// If the buffer has any data in it, add an ampersand to separate
		// this request from any previous requests.
		//
		buf += '&';
	}

	//
	// Append the data from the new request to the request buffer, and make
	// sure to put a terminator after it.
	//
	buf.append(newRequest);
	return true;
}

// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.cpp ==================
