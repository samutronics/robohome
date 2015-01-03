// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \note
// =============================================================================
#ifndef _OUTPUTMANAGER_H_
#define _OUTPUTMANAGER_H_

#include "iservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace outboundTask {

class output: public IService{
private: output();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(output)

private: xQueueHandle _queueOutbound;
};

} // outboundTask

} // manager

#endif // _OUTPUTMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: outputmanager.hpp ========================
