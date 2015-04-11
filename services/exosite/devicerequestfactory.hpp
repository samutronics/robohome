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
public: deviceRequestFactory();

public: const std::string& readRequest();
public: const std::string& writeRequest();
public: bool updateEntryByResponse(statisticEntry& entry, const std::string& response);

private: std::string writeRequestOutbound;
private: std::string readRequestOutbound;
private: std::vector<std::string>	_inputs;
};

} // exosite
} // service

#endif // _DEVICEREQUESTFACTORY_HPP_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.hpp ==================
