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
#include "sntp.h"
#include "time.h"
#include "ustdlib.h"


using namespace communication::ipc;
using namespace manager::rtcTask;

rtcManager::rtcManager() {
	HibernateEnableExpClk(system::currentSystemClockFrequency);
	HibernateCounterMode(HIBERNATE_COUNTER_24HR);

	std::tm t;
	HibernateCalendarSet((tm*)&t);
	t.tm_min = 0xFF;
	HibernateCalendarMatchSet(0, reinterpret_cast<tm*>(&t));
	HibernateIntClear(HIBERNATE_INT_PIN_WAKE | HIBERNATE_INT_LOW_BAT | HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntEnable(HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntRegister(&rtcManager::handlerTH);
	HibernateRTCEnable();

	sntp_init(&delegateProcessSNTPTime);
}

void rtcManager::task(void *pvParameters) {
	while(1) {
		// Query the queue handler due to performance reason.
		xQueueHandle queueHandle = ipcQueue::singleton().queue(rtcQueue);

		// The thread gives up its time-slice, if the TH queue is empty: there was no interrupt
		while(0 == uxQueueMessagesWaiting(queueHandle)) {taskYIELD();}

		// If item received, read it from the TH queue
		u32 time;
		xQueueReceive(queueHandle, &time, 0);

		std::tm t;
		ulocaltime(time, reinterpret_cast<tm*>(&t));

		// The hour has to be incremented, because the Budapest
		//	time zone is shifted by one related to the GMT.
		t.tm_hour++;
		HibernateRTCDisable();
		HibernateCalendarSet(reinterpret_cast<tm*>(&t));
		HibernateRTCEnable();

		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void rtcManager::delegateProcessSNTPTime(u32 time) {
	xQueueSendToBack(ipcQueue::singleton().queue(rtcQueue), &time, NULL);
}

void rtcManager::handlerTH() {
//	sntp_init(&delegateProcessSNTPTime);
	HibernateIntClear(HIBERNATE_INT_RTC_MATCH_0);
}

//! =============================================================================
//! \copyright
//! ===================== end of file: rtcmanager.cpp ========================
