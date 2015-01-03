// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#ifndef _LED_MANAGER_H_
#define _LED_MANAGER_H_

#include "iservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace inboundTask {

class input: public IService {
private: input();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(input)
DEFINE_TH
};

}  // inboundTask

}  // manager

#endif //_LED_MANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: inputmanager.hpp ==========================
