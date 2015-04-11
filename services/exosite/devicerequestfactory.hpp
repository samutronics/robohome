// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#ifndef _DEVICEREQUESTFACTORY_HPP_
#define _DEVICEREQUESTFACTORY_HPP_

#include "projectconfiguration.hpp"

namespace service {
namespace exosite {

class deviceRequestFactory {
public: deviceRequestFactory();

public: const std::string& readRequest();
public: const std::string& writeRequest();

private: std::string 				_writeRequestOutbound;
private: std::string 				_readRequestOutbound;
private: std::vector<std::string>	_inputs;
};

} // exosite
} // service

#endif // _DEVICEREQUESTFACTORY_HPP_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicerequestfactory.hpp ==================
