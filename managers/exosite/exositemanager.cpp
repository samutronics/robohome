// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#include "exosite.h"
#include "exositemanager.hpp"
#include "../projectconfiguration.hpp"

using namespace manager::exositeTask;

exositeManager::exositeManager() {
    Exosite_Init("texasinstruments", "ek-tm4c1294xl", IF_ENET, 0);
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
