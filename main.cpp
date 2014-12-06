//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			30.11.2014.
//! \details
//! \note
//! =============================================================================

// =============================================================================
// standard includes
// =============================================================================
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// FreeRTOS related includes -- porting for the given hardware
// =============================================================================
#include "portmacro.h"
#include "FreeRTOS.h"

// =============================================================================
// FreeRTOS related includes -- scheduler
// =============================================================================
#include "list.h"
#include "task.h"
#include "queue.h"

// =============================================================================
// Define, which TivaWare librabry is used by the ROM
// =============================================================================
#define TARGET_IS_TM4C129_RA1

// =============================================================================
// Drived librarie includes
// =============================================================================
#include "rom.h"

#include "hw_gpio.h"
#include "hw_types.h"
#include "hw_memmap.h"

#include "gpio.h"
#include "sysctl.h"
#include "pin_map.h"

// =============================================================================
// Custom include files
// =============================================================================
#include "inputmanager.hpp"

using namespace manager::task;

int main(void) {

	ROM_SysCtlMOSCConfigSet(SYSCTL_MOSC_HIGHFREQ);
	ROM_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);











/*
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);


    // PA0-1 are used for UART0.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        // this app wants to configure for ethernet LED function.
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        ROM_GPIOPinConfigure(GPIO_PF0_EN0LED0);
        ROM_GPIOPinConfigure(GPIO_PF4_EN0LED1);
//        ROM_GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);




            //
            // Make the pin(s) be peripheral controlled.
            //
            ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_DIR_MODE_HW);

            //
            // Set the pad(s) for standard push-pull operation.
            //
            ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);












    // PN0 and PN1 are used for USER LEDs.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//    ROM_GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);

    // Default the LEDs to OFF.
    ROM_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0);
*/



















	if(pdPASS != xTaskCreate(&inputManager::start, "TaaT_TBHB_Input",	configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
/*
	if(pdPASS != xTaskCreate(, "TaaT_Output",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(, "TaaT_WebServer",	configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(, "TaaT_ExositeIoT",	configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(, "TaaT_THBH_IP",		configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
	if(pdPASS != xTaskCreate(, "TaaT_RTC",			configUSER_SPACE_STACK_SIZE, NULL, 1, NULL)) { while(1);}
*/
	vTaskStartScheduler();
	while(1);
	return 0;
}
//! =============================================================================
//! \copyright
//! ========================= end of file: main.c ===============================
