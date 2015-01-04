// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#ifndef __REQUESTS_HPP__
#define __REQUESTS_HPP__

#include "basicvector.hpp"
#include "statisticentry.hpp"
#include "../../projectconfiguration.hpp"

namespace service {
namespace exositeTask {

class deviceRequestFactory {
public: static const u32 requestBufferSize = 255;

public: static bool makeDeviceSyncRequest();
public: static bool makeSyncRequest(const statisticEntry& entry);
public: static bool updateEntryByResponse(statisticEntry& entry);

private: static bool addRequest(const char* pcNewRequest, basicVector<u8, requestBufferSize>& buf, uint32_t ui32Size);

public: static basicVector<u8, requestBufferSize> writeRequestOutbound;
public: static basicVector<u8, requestBufferSize> readRequestOutbound;
public: static basicVector<u8, requestBufferSize> response;
};

} // exositeTask
} // namespace manager

#endif // __REQUESTS_HPP__
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.hpp ==================
