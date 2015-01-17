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

namespace service {
namespace exosite {

class deviceRequestFactory {
public: static const std::string& readRequest();
public: static const std::string& writeRequest();
public: static bool updateEntryByResponse(statisticEntry& entry);

public: static std::string response;

private: static bool addRequest(const std::string& newRequest, std::string& buf);

private: static std::string writeRequestOutbound;
private: static std::string readRequestOutbound;
};

} // exositeTask
} // namespace manager

#endif // __REQUESTS_HPP__
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.hpp ==================
