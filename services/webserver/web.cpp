// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#include "web.hpp"
#include "httpd.h"
#include "sntp.hpp"

using namespace service::webTask;
using namespace service::webTask::configuration;

web::web() {
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

    lwIPInit(systemGlobal::currentSystemClockFrequency, mac, 0, 0, 0, IPGatheringStrategie);
    while((0xFFFFFFFF == lwIPLocalIPAddrGet()) || (0x0 == lwIPLocalIPAddrGet())) {taskYIELD();}

    //Guard the message printing.
    taskENTER_CRITICAL();
    u32 IP = lwIPLocalIPAddrGet();
    UARTprintf("Device IP address: %d.%d.%d.%d\n", (IP & 0xff), ((IP >> 8) & 0xff), ((IP >> 16) & 0xff), ((IP >> 24) & 0xff));
    taskEXIT_CRITICAL();

    httpd_init();
}

void web::task(void *pvParameters) {
    while(1) {
		taskYIELD();
	}
}

// =============================================================================
//! \file
//! \copyright
// =================== end of file: netwoorkmanager.cpp ========================
