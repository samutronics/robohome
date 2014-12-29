// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _WEATHERMANAGER_H_
#define _WEATHERMANAGER_H_

#include "imanager.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace weatherTask {

class weatherManager: public IManager {
private: weatherManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(weatherManager)
DEFINE_TH
};

} // weatherTask

} // manager

#endif // _WEATHERMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: weathermanager.hpp ========================
