// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#include "exosite.hpp"
#include "requestFactory.hpp"
#include "devicestatistic.hpp"

using namespace manager::exositeTask;

basicVector<u8, requestFactory::requestBufferSize> requestFactory::writeRequestOutbound;
basicVector<u8, requestFactory::requestBufferSize> requestFactory::readRequestOutbound;
basicVector<u8, requestFactory::requestBufferSize> requestFactory::_response;

bool requestFactory::makeDeviceSyncRequest() {
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

bool requestFactory::extractValueByAlias(const char* pcAlias, char* pcBuffer, char* pcDestString, uint32_t ui32MaxSize) {
    char pcSearchString[100];
    char *pcValueStart;
    uint32_t ui32Idx;

    //
    // Set the search string to be the desired alias with an equals-sign
    // appended. This should help us distinguish between a real alias and a
    // string value made up of the same letters.
    //
    sprintf(pcSearchString, "%s=", pcAlias);

    //
    // Find the desired alias in the buffer.
    //
    pcValueStart = strstr(pcBuffer, pcAlias);

    //
    // If we couldn't find it, return a zero. Otherwise, continue extracting
    // the value.
    //
    if(!pcValueStart)
    {
        return false;
    }

    //
    // Find the equals-sign, which should be just before the start of the
    // value.
    //
    pcValueStart = strstr(pcValueStart, "=");

    if(!pcValueStart)
    {
        return false;
    }

    //
    // Advance to the first character of the value.
    //
    pcValueStart++;

    //
    // Loop through the input value from the buffer, and copy characters to the
    // destination string.
    //
    ui32Idx = 0;
    while(ui32Idx < ui32MaxSize)
    {
        //
        // Check for the end of the value string.
        //
        if((pcValueStart[ui32Idx] == '&') ||
           (pcValueStart[ui32Idx] == 0))
        {
            //
            // If we have reached the end of the value, null-terminate the
            // destination string, and return.
            //
            pcDestString[ui32Idx] = 0;
            return true;
        }
        else
        {
            pcDestString[ui32Idx] = pcValueStart[ui32Idx];
        }

        ui32Idx++;
    }

    pcDestString[ui32MaxSize - 1] = 0;
    return true;
}

bool requestFactory::makeSyncRequest(const statisticEntry& entry) {
    char pcFormattedRequest[100];

    //
    // Only interact with the server if the stat has an alias
    //
    if(entry.entryAliasInCloud == 0) {
        return true;
    }

    //
    // Check to see if we write this stat to the server.
    //
    if((entry.access == WRITE_ONLY) || (entry.access == READ_WRITE))
    {
        //
        // Format a request to write the current value of this stat.
        //
    	entry.requestFormat(pcFormattedRequest);

        //
        // If the request didn't fit, report failure.
        //
        if(!addRequest(pcFormattedRequest, writeRequestOutbound, strlen(pcFormattedRequest)))
        {
            return false;
        }

    }
    else if(entry.access == READ_ONLY) {
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

bool requestFactory::addRequest(const char* pcNewRequest, basicVector<u8, requestBufferSize>& buf, uint32_t ui32Size) {
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
