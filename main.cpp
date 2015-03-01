// =============================================================================
//! \file
//! \brief			Implementation of main() method
//! \author         Norbert Toth
//! \date			30.11.2014.
//! \details		The file targets to implement ONLY the main method. Any other
//!						compilable stuff should be in another files.
//! \note
// =============================================================================

// =============================================================================
// Custom include files
// =============================================================================
#include "web.hpp"
#include "sntp.hpp"
#include "output.hpp"
#include "weather.hpp"
#include "exosite.hpp"
#include "services/input/input.hpp"
#include "projectconfiguration.hpp"

using namespace service::sntp;
using namespace service::inbound;
using namespace service::web;
using namespace service::weather;
using namespace service::exosite;
using namespace service::outbound;

//! =============================================================================
//! The main method is responsible for:
//!		* Setup system wide availeble stuffs, that are not relating to another
//!			modules, like system clock
//!		* Create and deploy user threads
//!		* Start scheduler
//! =============================================================================
int main(void) {
	SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);
	systemGlobal::currentSystemClockFrequency = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTStdioConfig(0, 115200, systemGlobal::currentSystemClockFrequency);
    UARTprintf("\033[2J\033[H");
	UARTprintf("application starts\n");

	if(pdPASS != xTaskCreate(&input::start,		"TaaT_TBHB_Input",	configUSER_SPACE_STACK_SIZE, NULL, 2, NULL)) { while(true);}
	if(pdPASS != xTaskCreate(&output::start,	"TaaT_Output",		configUSER_SPACE_STACK_SIZE, NULL, 2, NULL)) { while(true);}
	if(pdPASS != xTaskCreate(&web::start,		"TaaT_THBH_NP",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(true);}
	if(pdPASS != xTaskCreate(&sntp::start,		"TaaT_RTC",			configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(true);}
	if(pdPASS != xTaskCreate(&weather::start,	"TaaT_WEATHER",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(true);}
	if(pdPASS != xTaskCreate(&exosite::start,	"TaaT_Exosite",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(true);}

	vTaskStartScheduler();
	while(true);
}
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: main.cpp ==============================
