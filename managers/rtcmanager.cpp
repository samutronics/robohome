//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			15.12.2014.
//! \details
//! \note
//! =============================================================================
#include "ipcQueue.hpp"
#include "rtcmanager.hpp"

using namespace communication::ipc;
using namespace manager::rtcTask;

rtcManager::rtcManager() {
}

void rtcManager::task(void *pvParameters) {
	while(1) {
		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void rtcManager::handlerTH() {
}

//! =============================================================================
//! \copyright
//! ===================== end of file: rtcmanager.cpp ========================
