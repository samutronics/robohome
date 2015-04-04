// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#ifndef _EXOSITE_H_
#define _EXOSITE_H_

#include "singletonfactory.hpp"
#include "devicerequestfactory.hpp"
#include "abstractclientservice.hpp"
#include "exositerequestfactory.hpp"
#include "projectconfiguration.hpp"

struct netconn;

namespace service {
namespace exosite {

// =============================================================================
//! \brief Service implementation to handle connection between the embedded
//!		device and the ti.exosite.com.
// =============================================================================
class Exosite: public abstractclientservice {
protected: virtual bool processingReply(netbuf* reply);
protected: virtual netbuf* generateRequest();

protected: inline Exosite();

// =============================================================================
// Member declarations
// =============================================================================
private: deviceRequestFactory	_deviceRequestFactory;
private: exositeRequestFactory	_exositeRequestFactory;
private: std::string			_workerBuffer;
private: bool					_requestPost;
};

typedef libs::SingletonFactory<Exosite> ExositeFactory;

inline Exosite::Exosite():
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
