// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#ifndef _NETWORKMANAGER_HPP_
#define _NETWORKMANAGER_HPP_

#include "imanager.hpp"
#include "eth_client.h"
#include "../projectconfiguration.hpp"

namespace manager {

namespace networkTask {

class networkManager: public IManager {
private: networkManager();
private: virtual void task(void *pvParameters);

private: static tWeatherReport _weatherReport;

private: static void cbWetherMethod(uint32_t ui32Event, void* pvData, uint32_t ui32Param);

TO_BE_RUNABLE(networkManager)
DEFINE_TH
};

} // networkTask

} // manager

#endif // _NETWORKMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// =================== end of file: netwoorkmanager.hpp ========================
