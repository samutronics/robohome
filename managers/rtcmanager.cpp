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


//! \note wokraround, there was an compiler failure, if I included the time.h
//!			and I tried to use its tm struct.

struct tm
{
    int tm_sec;      /* seconds after the minute   - [0,59]  */
    int tm_min;      /* minutes after the hour     - [0,59]  */
    int tm_hour;     /* hours after the midnight   - [0,23]  */
    int tm_mday;     /* day of the month           - [1,31]  */
    int tm_mon;      /* months since January       - [0,11]  */
    int tm_year;     /* years since 1900                     */
    int tm_wday;     /* days since Sunday          - [0,6]   */
    int tm_yday;     /* days since Jan 1st         - [0,365] */
    int tm_isdst;    /* Daylight Savings Time flag           */
};

rtcManager::rtcManager() {
	HibernateEnableExpClk(system::currentSystemClockFrequency);
	HibernateCounterMode(HIBERNATE_COUNTER_24HR);

	tm t;
	t.tm_min = 0xFF;
	HibernateCalendarMatchSet(0, &t);
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
	TimerIntClear(TIMER0_BASE, TIMER_RTC_MATCH);
}

//! =============================================================================
//! \copyright
//! ===================== end of file: rtcmanager.cpp ========================
