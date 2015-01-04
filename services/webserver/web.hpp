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

namespace service {
namespace webTask {

class web: public IService {
private: web();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(web)
};

} // networkTask

} // manager

#endif // _NETWORKMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// =========================== end of file: web.hpp ============================
