//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief			Implementation of main() method
//! \author         Norbert Toth
//! \date			30.11.2014.
//! \details		The file targets to implement ONLY the main method. Any other
//!						compilable stuff should be in another files.
//! \note
//! =============================================================================

// =============================================================================
// Custom include files
// =============================================================================
#include "inputmanager.hpp"
#include "outputManager.hpp"
#include "networkManager.hpp"
#include "projectconfiguration.hpp"

#include "lwiplib.h"

using namespace manager::inboundTask;
using namespace manager::networkTask;
using namespace manager::outboundTask;

//! =============================================================================
//! \function main is responsible for:
//!		* Setup system wide availeble stuffs, that are not relating to another
//!			modules, like system clock
//!		* Create and deploy user threads
//!		* Start scheduler
//! =============================================================================
int main(void) {
	SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);
	system::currentSystemClockFrequency = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinConfigure(GPIO_PF0_EN0LED0);
	GPIOPinConfigure(GPIO_PF4_EN0LED1);
	GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_DIR_MODE_HW);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

	u32 usr0;
	u32 usr1;
    FlashUserGet(&usr0, &usr1);
    if((usr0 == 0xffffffff) || (usr1 == 0xffffffff)) {while(1);}

    u8 mac[8];
    mac[0] = ((usr0 >>  0) & 0xff);
    mac[1] = ((usr0 >>  8) & 0xff);
    mac[2] = ((usr0 >> 16) & 0xff);
    mac[3] = ((usr1 >>  0) & 0xff);
    mac[4] = ((usr1 >>  8) & 0xff);
    mac[5] = ((usr1 >> 16) & 0xff);

    lwIPInit(system::currentSystemClockFrequency, mac, 0, 0, 0, manager::networkTask::configuration::IPGatheringStrategie);



//	if(pdPASS != xTaskCreate(&inputManager::start,	"TaaT_TBHB_Input",	configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
//	if(pdPASS != xTaskCreate(&outputManager::start,	"TaaT_Output",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
//	if(pdPASS != xTaskCreate(&networkManager::start, "TaaT_THBH_NP",	configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
/*
	if(pdPASS != xTaskCreate(, "TaaT_ExositeIoT",	configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(, "TaaT_WebServer",	configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(, "TaaT_RTC",			configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
*/
	vTaskStartScheduler();
	while(1);
}
//! =============================================================================
//! \copyright
//! ========================= end of file: main.c ===============================
