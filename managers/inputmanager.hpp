// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#ifndef _LED_MANAGER_H_
#define _LED_MANAGER_H_

#include "imanager.hpp"
#include "../projectconfiguration.hpp"

namespace manager {

namespace inboundTask {

class inputManager: public IManager {
private: inputManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(inputManager)
DEFINE_TH
};

}  // inboundTask

}  // manager

#endif //_LED_MANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: inputmanager.hpp ==========================
