//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			15.12.2014.
//! \details
//! \note
//! =============================================================================
#ifndef _RTCMANAGER_H_
#define _RTCMANAGER_H_

#include "imanager.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace rtcTask {

class rtcManager: public IManager{
private: rtcManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(rtcManager)
DEFINE_TH
};

} // rtcTask

} // manager

#endif _RTCMANAGER_H_
//! =============================================================================
//! \copyright
//! ====================== end of file: rtcmanager.hpp ==========================
