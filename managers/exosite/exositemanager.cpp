// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#include "exositemanager.hpp"
#include "../projectconfiguration.hpp"

using namespace manager::exositeTask;

exositeManager::exositeManager() {
}

void exositeManager::task(void *pvParameters) {
	while(1) {
		taskYIELD();
	}
}

// =============================================================================
//! \file
//! \copyright
// ===================== end of file: exositemanager.cpp =======================
