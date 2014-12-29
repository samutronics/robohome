// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#include "ipcQueue.hpp"
#include "weathermanager.hpp"

using namespace manager::weatherTask;

weatherManager::weatherManager() {
}

void weatherManager::task(void *pvParameters) {
	while(1) {
		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void weatherManager::handlerTH() {
}

// =============================================================================
//! \file
//! \copyright
// ===================== end of file: weathermanager.cpp =======================
