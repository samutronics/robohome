// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			15.12.2014.
//! \note
// =============================================================================
#ifndef _RTCMANAGER_H_
#define _RTCMANAGER_H_

#include "iservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace rtcTask {

class sntp: public IService {
private: sntp();
private: virtual void task(void *pvParameters);
private: static void delegateProcessSNTPTime(u32 time);
TO_BE_RUNABLE(sntp)
DEFINE_TH
};

} // rtcTask

} // manager

#endif // _RTCMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: sntp.hpp ============================
