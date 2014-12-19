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
#include "time.h"

using namespace communication::ipc;
using namespace manager::rtcTask;

rtcManager::rtcManager() {
	HibernateEnableExpClk(system::currentSystemClockFrequency);
	HibernateCounterMode(HIBERNATE_COUNTER_24HR);

	std::tm t;
	HibernateCalendarSet((tm*)&t);
	t.tm_min = 0xFF;
	HibernateCalendarMatchSet(0, (tm*)&t);
	HibernateIntClear(HIBERNATE_INT_PIN_WAKE | HIBERNATE_INT_LOW_BAT | HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntEnable(HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntRegister(&rtcManager::handlerTH);
	HibernateRTCEnable();
}

void rtcManager::task(void *pvParameters) {
	while(1) {
		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void rtcManager::handlerTH() {
	HibernateIntClear(HIBERNATE_INT_RTC_MATCH_0);
}

//! =============================================================================
//! \copyright
//! ===================== end of file: rtcmanager.cpp ========================
