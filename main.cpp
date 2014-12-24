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

#include "fatfs/src/ff.h"

static FATFS fs;
static FIL f;

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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTStdioConfig(0, 115200, system::currentSystemClockFrequency);
	UARTprintf("application starts\n");

	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinConfigure(GPIO_PQ3_SSI3XDAT1);
    GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
    GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
    GPIOPinConfigure(GPIO_PQ1_SSI3FSS);
    GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 |GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_1, GPIO_PIN_1); //select SD on the baseboard!!!
	UARTprintf("SSI for reading SD card is ready\n");

	if(FR_OK != f_mount(0, &fs)) {UARTprintf("fs cannot be mounted\n"); while(1);}

    if(FR_OK != f_open(&f, "/index.htm", FA_READ)) {UARTprintf("cannot open the htm file\n"); while(1);}
    UARTprintf("file size is %d\n", f_size(&f));

    void* pbuf = pvPortMalloc(f_size(&f));
    u32 count = 0;
    f_read(&f, (void*)pbuf, f_size(&f), &count);
    if(count) {
    	UARTwrite(reinterpret_cast<const char*>(pbuf), count);
    }


	if(pdPASS != xTaskCreate(&inputManager::start,		"TaaT_TBHB_Input",	configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(&outputManager::start,		"TaaT_Output",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(&networkManager::start,	"TaaT_THBH_NP",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
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
