// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#ifndef _EXOSITE_H_
#define _EXOSITE_H_

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

inline exosite::exosite():
		abstractclientservice(configuration::url, configuration::port, NETCONN_TCP, configuration::updatePeriode),
		_workerBuffer(1024, 0),
		_requestPost(true) {}

} // exosite
} // service

#endif // _EXOSITE_H_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.hpp ==========================
