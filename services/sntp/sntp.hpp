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

namespace service {
namespace sntp {

class sntp: public IService {
private: sntp();
private: virtual void task(void *pvParameters);
private: static void delegateProcessSNTPTime(u32 time);
TO_BE_RUNABLE(sntp)
DEFINE_TH
};

} // sntp

} // service

#endif // _SNTP_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: sntp.hpp ============================
