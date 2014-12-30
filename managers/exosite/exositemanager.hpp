// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#ifndef _EXOSITEMANAGER_H_
#define _EXOSITEMANAGER_H_

#include "imanager.hpp"

namespace manager {
namespace exositeTask {

class exositeManager: public IManager {
private: exositeManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(exositeManager)
};

} // exositeTask

} // manager

#endif // _EXOSITEMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: exositemanager.hpp ========================
