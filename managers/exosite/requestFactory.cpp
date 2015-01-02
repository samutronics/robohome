// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#include "request.hpp"
#include "exosite.hpp"
#include "devicestatistic.hpp"

using namespace manager::exositeTask;

s8 requestHandler::_writeRequestOutbound[requestBufferSize];
s8 requestHandler::_readRequestOutbound[requestBufferSize];
s8 requestHandler::_response[requestBufferSize];

bool requestHandler::syncWithExosite() {
    int eStatRWType[deviceStatistic::deviceStatisticEntryCount];
    char pcServerValue[100];
    uint32_t ui32Index = 0;

    //
    // Clear the request buffers
    //
    _readRequestOutbound[0] = 0;
    _writeRequestOutbound[0] = 0;

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
        eStatRWType[ui32Index++] = deviceStatistic::current()->access;
        if(addSyncRequest(*deviceStatistic::current())) {
            return false;
        }
    }

    //
    // Perform the write, and wait for a response from the server. If exosite
    // doesn't respond, return "false", and assume that no data got through.
    //
    if(strlen(_writeRequestOutbound)) {
//        if(!exosite::write(_writeRequestOutbound, ustrlen(_writeRequestOutbound))) {
//            return false;
//        }
    }

    //
    // Perform the read, and wait for a response from the server. If exosite
    // doesn't respond, return "false", and assume that no data got through.
    //
    if(strlen(_readRequestOutbound)) {
//        if(!exosite::read(_readRequestOutbound, _response, requestBufferSize)) {
//            return false;
//        }
    }

    //
    // If execution reaches this point, we can assume that the server has
    // accepted the data just sent. Update the information for each stat
    // accordingly.
    //

    ui32Index = 0;
    deviceStatistic::reset();
    while(deviceStatistic::next()) {

        //
        // Disable interrupts to avoid data disturbances in other interupt
        // contexts.
        //
        vPortEnterCritical();

        //
        // Check the read/write status of this stat.
        //
        if(deviceStatistic::current()->access == READ_ONLY)
        {
            //
            // If a stat is CURRENTLY marked as READ_ONLY, then update its
            // value to match the new value from the server.
            //
            extractValueByAlias(deviceStatistic::current()->entryAliasInCloud, _response, pcServerValue, 100);
            deviceStatistic::current()->setValue(pcServerValue);
        }
        else if(eStatRWType[ui32Index] == READ_WRITE)
        {
            //
            // If a stat was PREVIOUSLY marked as READ_WRITE at the time that
            // the most recent request was sent, then update its status to
            // READ_ONLY.
            //
        	deviceStatistic::current()->access = READ_ONLY;
        }

        //
        // Re-enable interrupts.
        //
        vPortExitCritical();
    }

    //
    // Successfully updated all stats.
    //
    return true;
}

bool requestHandler::extractValueByAlias(const char* pcAlias, char* pcBuffer, char* pcDestString, uint32_t ui32MaxSize) {
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

bool requestHandler::addSyncRequest(const statisticEntry& entry) {
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
        if(!addRequest(pcFormattedRequest, _writeRequestOutbound, strlen(pcFormattedRequest)))
        {
            return false;
        }

    }
    else if(entry.access == READ_ONLY) {
        //
        // If the request didn't fit, report failure.
        //
        if(!addRequest(entry.entryAliasInCloud, _readRequestOutbound, strlen(entry.entryAliasInCloud))) {
            return false;
        }
    }

    UARTprintf("The write request: %s\n", _writeRequestOutbound);
    UARTprintf("The reaad request: %s\n", _readRequestOutbound);

    //
    // Shouldn't get here...
    //
    return true;
}

bool requestHandler::addRequest(const char* pcNewRequest, char* pcRequestBuffer, uint32_t ui32Size) {
    uint32_t ui32BufferOffset;

    //
    // Set the buffer offset to the location of the first null character in the
    // string.
    //
    ui32BufferOffset = strlen(pcRequestBuffer);


    //
    // Check to make sure that the buffer is not full.
    //
    if(ui32BufferOffset >= requestBufferSize) {
        //
        // If the buffer was already full, return a zero to indicate failure.
        //
        pcRequestBuffer[requestBufferSize - 1] = 0;
        return false;
    }

    //
    // Check to make sure that the new request is small enough to fit in the
    // buffer, even if we have to add an ampersand and a null terminator.
    //
    if(ui32Size < (requestBufferSize - ui32BufferOffset - 2)) {
        if(ui32BufferOffset != 0) {
            //
            // If the buffer has any data in it, add an ampersand to separate
            // this request from any previous requests.
            //
            pcRequestBuffer[ui32BufferOffset++] = '&';
        }

        //
        // Append the data from the new request to the request buffer, and make
        // sure to put a terminator after it.
        //
        strncpy(&pcRequestBuffer[ui32BufferOffset], pcNewRequest, ui32Size);
        pcRequestBuffer[ui32BufferOffset + ui32Size] = 0;
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
