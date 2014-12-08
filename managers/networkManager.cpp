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

#include "networkManager.hpp"

using namespace manager::networkTask;

networkManager::networkManager() {

}

void networkManager::task(void *pvParameters) {
	while(1) {
		taskYIELD();
	}
}

void networkManager::handlerTH() {

}

//! =============================================================================
//! \copyright
//! =================== end of file: netwoorkmanager.cpp ========================
