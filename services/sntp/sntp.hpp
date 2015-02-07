// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			15.12.2014.
//! \note
// =============================================================================
#ifndef _SNTP_HPP_
#define _SNTP_HPP_

#include "abstractclientservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace sntp {

class sntp: public abstractclientservice {
	TO_BE_RUNABLE(sntp)

protected: virtual bool processingReply(netbuf* reply);
protected: virtual netbuf* generateRequest();

private: sntp();
DEFINE_TH
};

} // sntp
} // service

#endif // _SNTP_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: sntp.hpp ============================
