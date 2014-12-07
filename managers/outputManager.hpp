//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \details
//! \note
//! =============================================================================
#ifndef _OUTPUTMANAGER_H_
#define _OUTPUTMANAGER_H_

#include "imanager.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace task {

class outputManager: public IManager{
private: outputManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(outputManager)

private: xQueueHandle _queueOutbound;
};

} //task

} //manager

#endif // _OUTPUTMANAGER_H_
//! =============================================================================
//! \copyright
//! ===================== end of file: outputmanager.hpp ========================
