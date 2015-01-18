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
public: inline deviceRequestFactory();

public: const std::string& readRequest();
public: const std::string& writeRequest();
public: bool updateEntryByResponse(statisticEntry& entry, const std::string& response);

private: bool addRequest(const std::string& newRequest, std::string& buf);

private: std::string writeRequestOutbound;
private: std::string readRequestOutbound;
};

inline deviceRequestFactory::deviceRequestFactory() {
	writeRequestOutbound.reserve(255);
	readRequestOutbound.reserve(255);
}

} // exositeTask
} // namespace manager

#endif // __REQUESTS_HPP__
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.hpp ==================
