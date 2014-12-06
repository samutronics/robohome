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

#include "../FreeRTOS/include/FreeRTOS.h"
#include "../FreeRTOS/include/queue.h"

namespace manager {

namespace task {

class inputManager: public IManager {
	public: virtual ~inputManager();

	protected: inputManager();

	private: virtual void task(void *pvParameters);
	TO_BE_RUNABLE(inputManager)

	private: xQueueHandle _queueInbound;
};

}  // namespace tasks

}  // namespace manager

#endif //_LED_MANAGER_H_
//! =============================================================================
//! \copyright
//! ==================== end of file: inputmanager.hpp ==========================
