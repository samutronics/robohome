// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			15.12.2014.
//! \note
// =============================================================================
#include "time.h"
#include "sntpd.h"
#include "sntp.hpp"
#include "ipcQueue.hpp"

using namespace communication::ipc;
using namespace service::rtcTask;

sntp::sntp() {
	HibernateEnableExpClk(systemGlobal::currentSystemClockFrequency);
	HibernateCounterMode(HIBERNATE_COUNTER_24HR);

	tm t;
	memset(&t, 0, sizeof(tm));
	HibernateCalendarSet(&t);
	HibernateCalendarMatchSet(0, &t);
	HibernateIntClear(HIBERNATE_INT_PIN_WAKE | HIBERNATE_INT_LOW_BAT | HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntEnable(HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntRegister(&sntp::handlerTH);
	HibernateRTCEnable();

	sntp_init(&delegateProcessSNTPTime);
}

void sntp::task(void *pvParameters) {
	while(1) {
		// Query the queue handler due to performance reason.
		xQueueHandle queueHandle = ipcQueue::singleton().queue(rtcQueue);

		// The thread gives up its time-slice, if the TH queue is empty: there was no interrupt
		while(0 == uxQueueMessagesWaiting(queueHandle)) {taskYIELD();}

		// If item received, read it from the TH queue
		u32 time;
		xQueueReceive(queueHandle, &time, 0);

		tm* t = std::localtime(&time);

		// The hour has to be incremented, because the Budapest
		//	time zone is shifted by one related to the GMT.
		t->tm_hour++;

		// The TivaC library add 100 to the year.
		// It's incomprehensible, why...
		t->tm_year += 100;
		HibernateCalendarSet(t);

		//Guard the message printing.
		taskENTER_CRITICAL();
		HibernateCalendarGet(t);
		UARTprintf("Date and time is set to: %d.%d.%d %d:%d:%d\n",
				t->tm_mday,
				t->tm_mon + 1, // increment the month due to human readable format
				t->tm_year + 1970 - 100, // add the date offset: 1970, and subtract the library "compensation"
				t->tm_hour,
				t->tm_min,
				t->tm_sec);
		taskEXIT_CRITICAL();

		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void sntp::delegateProcessSNTPTime(u32 time) {
	xQueueSendToBack(ipcQueue::singleton().queue(rtcQueue), &time, NULL);
}

void sntp::handlerTH() {
	HibernateIntClear(HIBERNATE_INT_RTC_MATCH_0);
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: sntp.cpp ============================
