//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \details
//! \note
//! =============================================================================
#ifndef _LED_MANAGER_H_
#define _LED_MANAGER_H_

#include "imanager.hpp"
#include "../projectconfiguration.hpp"

namespace manager {

namespace task {

class inputManager: public IManager {
private: inputManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(inputManager)
};

}  // tasks

}  // manager

#endif //_LED_MANAGER_H_
//! =============================================================================
//! \copyright
//! ==================== end of file: inputmanager.hpp ==========================
