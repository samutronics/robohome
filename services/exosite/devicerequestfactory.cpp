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

string deviceRequestFactory::writeRequestOutbound;
string deviceRequestFactory::readRequestOutbound;
string deviceRequestFactory::response;

bool deviceRequestFactory::makeDeviceSyncRequest() {
    //
    // Clear the request buffers
    //
    readRequestOutbound.clear();
    writeRequestOutbound.clear();

    //
    // Loop over all statistics in the list, and add them to the request
    // buffer.
    //

    deviceStatistic::reset();
    while(deviceStatistic::next()) {
        //
        // Record the read/write behavior of each stat before sending the
        // request. If a particular stat is set to "READ_WRITE", we need to
        // know that now.
        //
        if(!makeSyncRequest(*deviceStatistic::current())) {
            return false;
        }
    }

    return true;
}

bool deviceRequestFactory::updateEntryByResponse(statisticEntry& entry) {
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

bool deviceRequestFactory::makeSyncRequest(const statisticEntry& entry) {
	//
	// Only interact with the server if the entry has an alias
	//
	if(entry.entryAliasInCloud == 0) {return true;}

	//
	// Check to see if we write this stat to the server.
	//
	if((entry.access == WRITE_ONLY) || (entry.access == READ_WRITE)) {
		//
		// Format a request to write the current value of this stat.
		//
		string str;
		str.reserve(100);
		entry.requestFormat(str);

		//
		// If the request didn't fit, report failure.
		//
		if(!addRequest(str, writeRequestOutbound)) {
			return false;
		}

	}

	if((entry.access == READ_ONLY) || (entry.access == READ_WRITE)) {
		//
		// If the request didn't fit, report failure.
		//
		if(!addRequest(entry.entryAliasInCloud, readRequestOutbound)) {
			return false;
		}
	}

	//
	// Shouldn't get here...
	//
	return true;
}

bool deviceRequestFactory::addRequest(const std::string& pcNewRequest, std::string& buf) {
	if(buf.length() != 0) {
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
	buf.append(pcNewRequest);
	return true;
}

// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.cpp ==================
