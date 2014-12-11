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
using namespace manager::networkTask::configuration;

DECLARE_TH(networkManager)

networkManager::networkManager() {
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

    lwIPInit(system::currentSystemClockFrequency, mac, 0, 0, 0, IPGatheringStrategie);

    LocatorInit();
    LocatorMACAddrSet(mac);
    LocatorAppTitleSet("automation");
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
