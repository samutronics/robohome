// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#ifndef _DEVICEREQUESTFACTORY_HPP_
#define _DEVICEREQUESTFACTORY_HPP_

#include "statisticentry.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace exosite {

class deviceRequestFactory {
public: inline deviceRequestFactory();

public: const std::string& readRequest();
public: const std::string& writeRequest();
public: bool updateEntryByResponse(statisticEntry& entry, const std::string& response);

private: bool addRequest(const std::string& newRequest, std::string& buf);

private: std::string writeRequestOutbound;
private: std::string readRequestOutbound;
};

inline deviceRequestFactory::deviceRequestFactory(): writeRequestOutbound(255, ' '), readRequestOutbound(255, ' ') {}

} // exosite
} // service

#endif // _DEVICEREQUESTFACTORY_HPP_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.hpp ==================
