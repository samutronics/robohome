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

using namespace service::exositeTask;

basicVector<u8, deviceRequestFactory::requestBufferSize> deviceRequestFactory::writeRequestOutbound;
basicVector<u8, deviceRequestFactory::requestBufferSize> deviceRequestFactory::readRequestOutbound;
basicVector<u8, deviceRequestFactory::requestBufferSize> deviceRequestFactory::response;

bool deviceRequestFactory::makeDeviceSyncRequest() {
    //
    // Clear the request buffers
    //
    readRequestOutbound.len = 0;
    writeRequestOutbound.len = 0;

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
    char *pcValueStart;

    //
    // Find the desired alias in the buffer.
    //
    pcValueStart = strstr((s8*)response.container, entry.entryAliasInCloud);

    //
    // If we couldn't find it, return a zero. Otherwise, continue extracting
    // the value.
    //
    if(!pcValueStart) {return false;}

    //
    // Find the equals-sign, which should be just before the start of the
    // value.
    //
    pcValueStart = strstr(pcValueStart, "=");

    if(!pcValueStart) {return false;}

    //
    // Advance to the first character of the value.
    //
    pcValueStart++;

    //
    // Loop through the input value from the buffer, and copy characters to the
    // destination string.
    //
    s8 pcDestString[statisticEntry::dataStringLength];
    for(u32 index = 0; index < response.len; index++) {
        //
        // Check for the end of the value string.
        //
        if((pcValueStart[index] == '&') || (pcValueStart[index] == 0)) {
            //
            // If we have reached the end of the value, null-terminate the
            // destination string, and return.
            //
            pcDestString[index] = 0;
            entry.setValue(pcDestString);
            return true;
        }
        else {
            pcDestString[index] = pcValueStart[index];
        }
    }

    return true;
}

bool deviceRequestFactory::makeSyncRequest(const statisticEntry& entry) {
	char pcFormattedRequest[100];

	//
	// Only interact with the server if the stat has an alias
	//
	if(entry.entryAliasInCloud == 0) {return true;}

	//
	// Check to see if we write this stat to the server.
	//
	if((entry.access == WRITE_ONLY) || (entry.access == READ_WRITE)) {
		//
		// Format a request to write the current value of this stat.
		//
		entry.requestFormat(pcFormattedRequest);

		//
		// If the request didn't fit, report failure.
		//
		if(!addRequest(pcFormattedRequest, writeRequestOutbound, strlen(pcFormattedRequest))) {
			return false;
		}

	}

	if((entry.access == READ_ONLY) || (entry.access == READ_WRITE)) {
		//
		// If the request didn't fit, report failure.
		//
		if(!addRequest(entry.entryAliasInCloud, readRequestOutbound, strlen(entry.entryAliasInCloud))) {
			return false;
		}
	}

	//
	// Shouldn't get here...
	//
	return true;
}

bool deviceRequestFactory::addRequest(const char* pcNewRequest, basicVector<u8, requestBufferSize>& buf, uint32_t ui32Size) {
    //
    // Check to make sure that the buffer is not full.
    //
    if(buf.len >= requestBufferSize) {
        //
        // If the buffer was already full, return a zero to indicate failure.
        //
        buf.container[buf.len - 1] = 0;
        return false;
    }

    //
    // Check to make sure that the new request is small enough to fit in the
    // buffer, even if we have to add an ampersand and a null terminator.
    //
    if(ui32Size < (requestBufferSize - buf.len - 2)) {
        if(buf.len != 0) {
            //
            // If the buffer has any data in it, add an ampersand to separate
            // this request from any previous requests.
            //
            buf.container[buf.len++] = '&';
        }

        //
        // Append the data from the new request to the request buffer, and make
        // sure to put a terminator after it.
        //
        strncpy((s8*)(buf.container + buf.len), pcNewRequest, ui32Size);
        buf.len += ui32Size;
        buf.container[buf.len] = 0;
        return true;
    }
    else
    {
        //
        // If the input string is too long, return a zero.
        //
        return false;
    }
}
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: request.cpp ==========================
