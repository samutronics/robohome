// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#ifndef _EXOSITEMANAGER_H_
#define _EXOSITEMANAGER_H_

#include "iservice.hpp"
#include "devicerequestfactory.hpp"
#include "exositerequestfactory.hpp"
#include "../../projectconfiguration.hpp"

struct netconn;

namespace service {
namespace exosite {

class exosite: public IService {
private: exosite();
private: virtual void task(void *pvParameters);
private: void retryContext(netconn*& connection, s32& error);
TO_BE_RUNABLE(exosite)

// =============================================================================
// Member declarations
// =============================================================================
private: deviceRequestFactory	_deviceRequestFactory;
private: exositeRequestFactory	_exositeRequestFactory;
private: std::string			_workerBuffer;
private: ip_addr				_serverIP;
};

} // exosite
} // manager

#endif // _EXOSITEMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.hpp ==========================
