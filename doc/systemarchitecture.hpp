// =============================================================================
//! \file
//! \brief			Documentation page
//! \author         Norbert Toth
//! \date			22.12.2014.
//! \note
// =============================================================================
//!
//! \page systemarchitecture System architecture
//! \tableofcontents
//!
//!	\section designconstraints Design constraints
//! -# There is no separeted input for the services. Services act as an write request from the input side.
//! -# The number of inputs and outputs are must be identical as the IO points in the hardware configuration
//! -# The project represented in binray form that will be processed by the system is valid and consistent.
//!
//!	\section projectmanagement Project management
//! The project is the human readable, text represented form that determines the run-time behavioral of the system related to its inputs and outputs.
//!
//!	\section ioarchitecture IO architecture
//!	The most important part of the system is the IO management. It's composed of two parts, the run-time model and the configuration management.
//! The run-time management covers the class and object model of run-time classes, properties and interactions with each other. The
//! configuration management determines how the compiled projects are interpreted and transformed into run-time objects.
//!
//! \subsection runtime Run-time model
//! There are two major requiremnts to the IO handling:
//! * The ablity to the IO reconfiguring both their properties and/or their numbers.
//! * The high performance in relation of the evaluation time.
//!
//! Observing the real use cases, the user required properties are represented at the ouptput side. Furthermore, the outputs are stateless objects,
//! that means that they don't hold theirs output states. Because of timed ouptputs can be represented in the system, decrease of theirs timers must
//! be guaranted. In order to be done, the following options have to be considered:
//! -# The processing goes through all of the inputs and each input objects notify the subscribed outputs.
//!		* pros:
//! 		* easy to implement
//! 		* easy to deploy a new project
//! 	* cons:
//! 		* the processing has to go trough all of the inputs even if there wasn't input change.
//! -# Every kind of timed outputs are stored in separeted arrays encapsulated of the input that's processed in separete step.
//! 	* pros:
//! 		* the non-timed outputs won't be evaluated
//! 	* cons:
//! 		* deployment sequence is more complicated
//! 		* a bit higher memory consumption because the memory allocation has been done in more steps
//! .
//! Considering the performance requirements, the second solution is prefered. It resoults the following evaluation sequence:
//!
//!
//! \subsubsection input Input
//! The main interaction of the inputs is the write operation on that from the direction of services. The main consumer
//! service is the IOTask that reads datas from the input boards and assigns it to the inputs. The communications is done
//! via SPI that's byte oriented.<br>
//! Considering the facts above, the SimpleInput class encapsualtes two array to store the current and the previous value
//! of the given input. The arrays are related to class instead of the object to be guaranted the uniqness whitin the
//! system. Two kind of write operation has to be possible relating to the inputs:
//! * Write a single bit is needed by the services, like webservice.
//! * Write a mass data is required for the IO task.
//! The inputs hold a trigger property that determines the kind of input which will be interpreted as input change. If an
//! appropriate change occured considering the trigger property, the connected outputs will be notified.
//!
//! \startuml
//! enum triggerMode {
//! triggerBothEdges
//! triggerRisingEdges
//! triggerFallingEdges
//! }
//!
//! class SimpleInput {
//! - triggerMode _trigger;
//! - std::vector<SimpleOutput*> _connected;
//! - {static} std::vector<unsigned char> _inputCurrent;
//! - {static} std::vector<unsigned char> _inputPrevious;
//! + SimpleInput(triggerMode t);
//! + void write();
//! + {static} void write(std::vector<unsigned char>& massData);
//! + void notifyOutputs();
//! }
//!
//! \enduml
//!
//! \subsubsection output Output
//! The system has two kind of output that can fulfill every requirements. Like SimpleInput above, the real data is in
//! a static array, and its address stored in the SimpleOutput object. Optionally, a time agument passed to the notification
//! can deffere the change of output cames into effect on the hardware.<br>
//! Construct time an argument adjust the timing behavioral of the output. If this parameter is zero, the timer property
//! is turned off. The following timing behavioral has to be implemented by the SimpleOutput class:
//!
//! Deferred timer  |   Timeout | Object state  |   Output
//! --------------- | --------- | ------------- | ---------
//!         On      |   On      |   passive     |   x
//!         On      |   Off     |   passive     |   x
//!         Off     |   On      |   passive     |   x
//!         Off     |   Off     |   passive     |   x
//!         On      |   On      |   active      |   x
//!         On      |   Off     |   active      |   x
//!         Off     |   On      |   active      |   x
//!         Off     |   Off     |   active      |   x
//!         On      |   On      |   wait        |   x
//!         On      |   Off     |   wait        |   x
//!         Off     |   On      |   wait        |   x
//!         Off     |   Off     |   wait        |   x
//!         On      |   On      |   activeUP    |   x
//!         On      |   Off     |   activeUP    |   x
//!         Off     |   On      |   activeUP    |   x
//!         Off     |   Off     |   activeUP    |   x
//!         On      |   On      |   activeDown  |   x
//!         On      |   Off     |   activeDown  |   x
//!         Off     |   On      |   activeDown  |   x
//!         Off     |   Off     |   activeDown  |   x
//!
//!
//! \startuml
//!
//! enum outputState {
//! passive
//! active
//! wait
//! activeUP
//! activeDown
//! }
//!
//! class SimpleOutput {
//! # outputState _state;
//! # unsigned short _hwAddress;
//! # unsigned short _timeOut;
//! # unsigned short _timer;
//! # unsigned short _deferredTimer;
//! # {static} std::vector<unsigned char> _output;
//! + SimpleOutput(unsigned short timeOut);
//! + notify(unsigned short deferredTime);
//! }
//!
//! class TriStateOutput {
//! - unsigned short _extendedHwAddress;
//! - unsigned char _stateChangeDelay;
//! }
//!
//! SimpleOutput <|-- TriStateOutput
//! \enduml
//!
//!
//! \subsection configuration Configuration management
//! By the CM the commissioner is able to change the behavioral of the system. The CM allows the change of parameters, that
//! aren't lead directly to a system crash. These parameters could be e.g. the URL of ntp server, the number inputs, connections
//! between in and outputs.
//!
//! \subsubsection datastructure Data Structure
//! A project represented in binary form is splitted up into the following main sections. That means that the sections also can
//! contains furthere subsections, but the interpretation of that belongs to the appropriate subsystem. The main sections are:
//! * system configuration
//! * input properties
//! * output properties
//! * irrigation description
//! .
//! The project structure highly inspired by partitioning table of the HDD's. So at the start of the project there will be the
//! offsets of sections listed above. That composed of the following internal structures:
//! * <b>System Configuration:</b> provides the following fields:
//!		* IP address gathering strategie, in case of static IP, the IP address, subnet mask and default gateway. The input section
//!		consist of the following fields:
//!			* IP address on 4 byte, is it's zero, the system assume a DHCP strategie
//!			* subnet mask on 4 byte, considered only in case of static IP
//!			* default gateway on 4 byte, considered only in case of static IP
//!			.
//!		.
//! * <b>Input properties:</b> section contains the properties of inputs. The list of inputs have to be mapped to the physical
//! address, e.g.: first item assign to the input bit 0, tenth item assign to the input bit 11. There isn't allowed tobe gap between
//! the inputs. The input section consist of the following fields:
//! 	* total number of inputs represented represented on unsigned short
//!		* list of trigger types: no more fields need to create an Input object
//!		.
//! * <b>Output properties:</b> in the current implementation splitted up to two more subsections: first for the simple output and
//! the second describes the tri-state outputs. The input section consist of the following fields:
//!		* start address of simple outputs represented on unsigned short
//!		* start address of tri-state outputs represented on unsigned short
//!		* simple output section:
//!			* number of simple outputs represented on unsigned short
//!			* lists of simple outputs:
//!				* address represented on unsigned short
//!				* time out represented on unsigned short
//!				* number of connected inputs represented on unsigned short
//!				* lists of address of inputs represented on unsigned short
//!				.
//!			.
//!		* tri-state output section:
//!			* number of tri-state outputs represented on unsigned short
//!			* lists of tri-state outputs:
//!				* address represented on unsigned short
//!				* time out represented on unsigned short
//!				* number of connected Up/Down inputs represented on unsigned short
//!				* lists of address of Up/Down inputs represented on unsigned short
//!				* extended address represented on unsigned short
//!				* number of connected Down inputs represented on unsigned short
//!				* lists of address of Down inputs represented on unsigned short
//!				* number of connected Up inputs represented on unsigned short
//!				* lists of address of Up inputs represented on unsigned short
//!				.
//!			.
//! * <b>Irrigation description:</b>
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
// ================= end of file: systemarchitecture.hpp =======================
