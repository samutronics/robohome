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

namespace manager {
namespace exositeTask {

class requestFactory {
public: static const u32 requestBufferSize = 255;

public: static bool makeDeviceSyncRequest();
public: static bool makeSyncRequest(const statisticEntry& entry);

private: static bool addRequest(const char* pcNewRequest, basicVector<u8, requestBufferSize>& buf, uint32_t ui32Size);
private: static bool extractValueByAlias(const char* pcAlias, char* pcBuffer, char* pcDestString, uint32_t ui32MaxSize);

public: static basicVector<u8, requestBufferSize> writeRequestOutbound;
public: static basicVector<u8, requestBufferSize> readRequestOutbound;
public: static basicVector<u8, requestBufferSize> _response;
};

} // exositeTask
} // namespace manager

#endif // __REQUESTS_HPP__
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: requestfactory.hpp =======================
