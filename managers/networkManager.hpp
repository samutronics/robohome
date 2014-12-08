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
#ifndef _NETWORKMANAGER_HPP_
#define _NETWORKMANAGER_HPP_

#include "imanager.hpp"
#include "../projectconfiguration.hpp"

namespace manager {

namespace networkTask {

class networkManager: public IManager {
private: networkManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(networkManager)
DEFINE_TH
};

} // networkTask

} // manager

#endif // _NETWORKMANAGER_HPP_
//! =============================================================================
//! \copyright
//! =================== end of file: netwoorkmanager.hpp ========================
