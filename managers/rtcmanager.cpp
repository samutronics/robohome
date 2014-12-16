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
	SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
	HibernateEnableExpClk(system::currentSystemClockFrequency);
	SysCtlDelay(1000000);
	HibernateClockConfig(HIBERNATE_OSC_LOWDRIVE);
	HibernateRTCEnable();
	//
	// Set the RTC to an initial value.
	//
	HibernateRTCSet(0);
	//
	// Set Match 0 for 30 seconds from now.
	//
	HibernateRTCMatchSet(0, 30);
	//
	// Set up interrupts on the Hibernation module to enable the RTC match
	// 0 interrupt. Clear all pending interrupts and register the
	// interrupt handler.
	//
	HibernateIntEnable(HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntClear(HIBERNATE_INT_PIN_WAKE | HIBERNATE_INT_LOW_BAT | HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntRegister(&rtcManager::handlerTH);
}

void rtcManager::task(void *pvParameters) {
	while(1) {
		// The task gives up its remained time-slice
		taskYIELD();
	}
}

void rtcManager::handlerTH() {
	while(true);
	TimerIntClear(TIMER0_BASE, TIMER_RTC_MATCH);
}

//! =============================================================================
//! \copyright
//! ===================== end of file: rtcmanager.cpp ========================
