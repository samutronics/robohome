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
#include "basicvector.hpp"
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
private: basicVector<u8, 4096>	_rxTxBuf;
private: ip_addr				_serverIP;
};

} // exosite

} // manager

#endif // _EXOSITEMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.hpp ==========================
