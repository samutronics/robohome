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
#include "../projectconfiguration.hpp"
#include "abstractservicerequesttransmitter.hpp"

namespace service {
namespace sntp {

class sntp: public IService, public communication::asbtractServiceRequestTransmitter {
	TO_BE_RUNABLE(sntp)

protected: virtual bool processingReply(netbuf* reply);
protected: virtual netbuf* generateRequest();

private: sntp();
private: virtual void task(void *pvParameters);
DEFINE_TH
};

} // sntp
} // service

#endif // _SNTP_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: sntp.hpp ============================
