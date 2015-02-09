// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#ifndef _EXOSITEMANAGER_H_
#define _EXOSITEMANAGER_H_

#include "devicerequestfactory.hpp"
#include "abstractclientservice.hpp"
#include "exositerequestfactory.hpp"
#include "../../projectconfiguration.hpp"

struct netconn;

namespace service {
namespace exosite {

// =============================================================================
//! \brief Service implementation to handle connection between the embedded
//!		device and the ti.exosite.com.
// =============================================================================
class exosite: public abstractclientservice {
	TO_BE_RUNABLE(exosite)

protected: virtual bool processingReply(netbuf* reply);
protected: virtual netbuf* generateRequest();

private: inline exosite();

// =============================================================================
// Member declarations
// =============================================================================
private: deviceRequestFactory	_deviceRequestFactory;
private: exositeRequestFactory	_exositeRequestFactory;
private: std::string			_workerBuffer;
private: bool					_requestPost;
};

inline exosite::exosite(): abstractclientservice(configuration::url, configuration::port, NETCONN_TCP, configuration::updatePeriode), _requestPost(true) {
	_workerBuffer.reserve(1024);
	u8 pucMACAddr[6];
	EMACAddrGet(EMAC0_BASE, 0, pucMACAddr);
	_exositeRequestFactory.init("texasinstruments", "ek-tm4c1294xl", IF_ENET, pucMACAddr, 0);
}

} // exosite
} // manager

#endif // _EXOSITEMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.hpp ==========================
