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
#include "ff.h"
#include "web.hpp"
#include "sntp.hpp"
#include "weather.hpp"
#include "exosite.hpp"
#include "irrigation.hpp"
#include "taskfactory.hpp"
#include "projectmanager.hpp"
#include "services/input/input.hpp"
#include "projectconfiguration.hpp"
#include "services/output/output.hpp"

using namespace systemGlobal;
using namespace service::sntp;
using namespace service::inbound;
using namespace service::web;
using namespace service::weather;
using namespace service::exosite;
using namespace service::outbound;
using namespace service::irrigation;

//! =============================================================================
//! The main method is responsible for:
//!		* Setup system wide availeble stuffs, that are not relating to another
//!			modules, like system clock
//!		* Create and deploy user threads
//!		* Start scheduler
//! =============================================================================
int main(void) {
	SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);
	currentSystemClockFrequency = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), requestedSystemClockFrequency);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinConfigure(GPIO_PD4_U2RX);
    GPIOPinConfigure(GPIO_PD5_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);

//	UARTStdioConfig(0, 115200, currentSystemClockFrequency);
	UARTStdioConfig(2, 115200, currentSystemClockFrequency);
    UARTprintf("\033[2J\033[H");
	UARTprintf("Application starts\n");

	static FATFS fileSystemMountPoint;
	if(FR_OK != f_mount(0, &fileSystemMountPoint)) {UARTprintf("fs cannot be mounted\n"); while(true);}

	UARTprintf("Project found\n");
	manager::project::ProjectManagerFactory::get()->parse();

	if(pdPASS != xTaskCreate(&libs::TaskFactory<InputFactory>::start,		"TaaT_TBHB_Input",	configUSER_SPACE_STACK_SIZE, NULL, 2, NULL)) { while(true);}
	if(pdPASS != xTaskCreate(&libs::TaskFactory<OutputFactory>::start,		"TaaT_Output",		configUSER_SPACE_STACK_SIZE, NULL, 2, NULL)) { while(true);}
	if(pdPASS != xTaskCreate(&libs::TaskFactory<IrrigationFactory>::start,	"TaaT_Irrigation",	configUSER_SPACE_STACK_SIZE, NULL, 2, NULL)) { while(true);}
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
