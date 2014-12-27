// =============================================================================
//! \file
//! \brief			Documentation page
//! \author         Norbert Toth
//! \date			22.12.2014.
//! \note
// =============================================================================

//! \page sds SDS
//! \tableofcontents
//! \section requirement Requirements
//!
//!	\section designconstraints Design constraints
//! There are several design rules and contrains for developing embedded systems, so we can discuss only about the
//! most importants.
//!
//!
//!





Specification:
	Healt signal LED
		* threading model: IaaT
		* onboard LED: PN0
		* blinking 1Hz periodicity
		* in case of error: continous lighting
	Pushbutton:
		* onboard pushbutton: PJ0
	Ethernet / IP configuration:
		IP address: 192.168.1.200
		Netmask: 255.255.255.0
		Default gateway: 192.168.1.0
    Onboard LED is controlled (ON/OFF) with
	    * onboard LED: PN1
		* the onboard pushbutton
		* over Web interface
		* Exosite IoT interface
	Weather information, RTC, LED status is on:
		* Web interface
		* Exosite IoT interface
	Current time is updated via SNTP:
		* power-up
		* 1h period --> RTC is setup accordingly (without losing clock ticks e.g. speedup or slowdown)
		* Hibernate module will provide the RTC functionality in the system.
	Web interface is located on an SD card with FAT32 FS

Threading model:
	* Separate TaaT + THBH for controling Input
	* Separate TaaT for controling Output
	* Separate TaaT for managing Web server
	* Separate TaaT for managing Exosite IoT
	* Separate TaaT + THBH for processing IP trafic
	* Separate TaaT for managing RTC

Behavioral models:
	Onboard LED:
		* threading model: TaaT
		* event received: turn ON or OFF
			PBStateCmd	PBStatus	PBStatus
			================================
				ON		|	ON		|	ON
				ON		|	OFF		|	ON
				OFF		|	ON		|	OFF
				OFF		|	OFF		|	OFF
		* LED handler post its new state
		
	Onboard pushbutton:
		* threading model: IaaT
		* post new event: LED_ON or LED_OFF
		* jitter handling: clarification is required 

	Input task:
		* THBH model
		* TH is an interrupt, which put the datas from the outside into the queue of the bottom side
		* BH is basicaly TaaT, wich process datas from its input queue
		* target: receive dedicated pushbutton change
		
	Output task:
		* infinity loop
		* quiryable input queue
		* target: change the state of the dedicated LED acording to the input queue
		

Useful examples:
	FreeRTOS demo (Warning this is for c129x board) *TivaWare*\examples\boards\dk-tm4c129x\freertos_demo\
	FATFS for SD card (Warning this is for f232 board)*TivaWare*\examples\boards\ek-lm4f232\sd_card\
	LwIP with Weather forecast *TivaWare*\examples\boards\ek-tm4c1294xl\enet_weather
	SNTP available on the net for FreeRTOS (no Tiva specific example available)
	Web server *TivaWare*\examples\boards\ek-tm4c1294xl\enet_io
	Exosite IoT *TivaWare*\examples\boards\ek-tm4c1294xl\qs_iot
	Flashing over Ethernet (Warning this is for c129x board) *TivaWare*\examples\boards\dk-tm4c129x\boot_demo_emac_flash

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: SDS.hpp ==============================
