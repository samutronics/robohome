// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			15.12.2014.
//! \note
// =============================================================================
#ifndef _SNTP_HPP_
#define _SNTP_HPP_

#include "iservice.hpp"
#include "abstractclientservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace sntp {

// =============================================================================
//! \brief Service implementation to ensure the system time to be up-to-date.
// =============================================================================
class Sntp: public abstractclientservice {

protected: virtual bool processingReply(netbuf* reply);
protected: virtual netbuf* generateRequest();

protected: Sntp();
DEFINE_TH
};

typedef libs::SingletonFactory<Sntp> SntpFactory;

} // sntp
} // service

#endif // _SNTP_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: sntp.hpp ============================
