// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#ifndef _NETWORKMANAGER_HPP_
#define _NETWORKMANAGER_HPP_

#include "iservice.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace networkTask {

class networkManager: public IManager {
private: networkManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(networkManager)
};

} // networkTask

} // manager

#endif // _NETWORKMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// =================== end of file: netwoorkmanager.hpp ========================
