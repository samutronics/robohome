// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#ifndef __REQUESTS_HPP__
#define __REQUESTS_HPP__

#include "statisticentry.hpp"
#include "../../projectconfiguration.hpp"

namespace manager {
namespace exositeTask {

class requestHandler {
public: static const u32 requestBufferSize = 255;

public: static bool syncWithExosite();

private: static bool addSyncRequest(const statisticEntry& entry);
private: static bool addRequest(const char* pcNewRequest, char* pcRequestBuffer, uint32_t ui32Size);
private: static bool extractValueByAlias(const char* pcAlias, char* pcBuffer, char* pcDestString, uint32_t ui32MaxSize);

private: static s8 _writeRequestOutbound[requestBufferSize];
private: static s8 _readRequestOutbound[requestBufferSize];
private: static s8 _response[requestBufferSize];
};

} // exositeTask
} // namespace manager

#endif // __REQUESTS_HPP__
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: request.hpp ==========================
