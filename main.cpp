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
#include "rtcmanager.hpp"
#include "inputmanager.hpp"
#include "outputManager.hpp"
#include "networkManager.hpp"
#include "projectconfiguration.hpp"

using namespace manager::rtcTask;
using namespace manager::inboundTask;
using namespace manager::networkTask;
using namespace manager::outboundTask;

//! =============================================================================
//! \fn
//! The main method is responsible for:
//!		* Setup system wide availeble stuffs, that are not relating to another
//!			modules, like system clock
//!		* Create and deploy user threads
//!		* Start scheduler
//! =============================================================================
int main(void) {
	SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);
	system::currentSystemClockFrequency = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTStdioConfig(0, 115200, system::currentSystemClockFrequency);
	UARTprintf("application starts\n");

	if(pdPASS != xTaskCreate(&inputManager::start,		"TaaT_TBHB_Input",	configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(&outputManager::start,		"TaaT_Output",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(&networkManager::start,	"TaaT_THBH_NP",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(&rtcManager::start,		"TaaT_RTC",			configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}

	vTaskStartScheduler();
	while(1);
}
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: main.cpp ==============================
