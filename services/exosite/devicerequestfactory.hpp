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
namespace exosite {

class deviceRequestFactory {
public: static const u32 requestBufferSize = 255;

public: static bool makeDeviceSyncRequest();
public: static bool makeSyncRequest(const statisticEntry& entry);
public: static bool updateEntryByResponse(statisticEntry& entry);

private: static bool addRequest(const std::string& pcNewRequest, std::string& buf);

public: static std::string writeRequestOutbound;
public: static std::string readRequestOutbound;
public: static std::string response;
};

} // exositeTask
} // namespace manager

#endif // __REQUESTS_HPP__
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.hpp ==================
