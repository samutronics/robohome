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
//! service is the Input that reads datas from the input boards and assigns it to the inputs. The communications is done
//! via SPI that's byte oriented.<br>
//! Considering the facts above, the SimpleInput class encapsualtes two array to store the current and the previous value
//! of the given input. The arrays are related to class instead of the object to be guaranted the uniqness whitin the
//! system. Two kind of write operation has to be possible relating to the inputs:
//! * Write a single bit is needed by the services, like webservice.
//! * Write a mass data is required for the Input task.
//! The kind of inputs determined by a trigger property.
//!
//! \subsubsection simpleoutput Simple output
//! This is the fundemantal type of outputs that can control a lamp or other simple devices.</br>
//! <b>Evaluation sequence:</b>
//! * check, whater time-out is configured
//!     * decrement if needed
//!     * evaluate the output bit
//!     .
//! * iterate trough the inputs
//!     * check the state-change property on that
//!     * evaluate the first occurance and break
//!     .
//!
//!
//!
//!
//!
//!
//! \subsection configuration Configuration management
//! By the CM the commissioner is able to change the behavioral of the system. The CM allows the change of parameters, that
//! aren't lead directly to a system crash. These parameters could be e.g. the URL of ntp server, the number inputs, connections
//! between in and outputs.
//!
//! \subsubsection datastructure Data Structure
//! The main structural unit is the section. The term section represents a "flat" collection of logilac related properties. There is
//! no restriction the properties to be the same types. Flat means in this context, that the sections can be embedded into another,
//! but in this case a section table has to be passed at the front of the new section level. It guarantees the minimal effort to
//! reach embedded sections.
//! A project represented in binary form is splitted up into four main sections:
//! * system configuration
//! * input properties
//! * output properties
//! * irrigation description
//! .
//! The project structure highly inspired by partitioning table of the HDD's. So at the start of the project there will be the
//! offsets of sections listed above. That composed of the following internal structures:
//! * <b>System Configuration:</b> provides the following fields:
//!     * available hardware inputs. It has to be a number of multiple of 8 and represented on two bytes.
//!     * available hardware outputs. It has to be a number of multiple of 8 and represented on two bytes.
//!		* IP configuration:
//!			* IP address on 4 byte, if it's zero, the system assume a DHCP strategie
//!			* subnet mask on 4 byte, considered only in case of static IP
//!			* default gateway on 4 byte, considered only in case of static IP
//!			.
//!		.
//! * <b>Input properties:</b> section contains the properties of inputs. The list of inputs have to be mapped to the physical
//! address, e.g.: first item assign to the input bit 0, tenth item assign to the input bit 11. There isn't allowed tobe gap between
//! the inputs. The input section consist of the following fields:
//! 	* total number of inputs represented represented on unsigned short
//!		* list of trigger types represented on unsigned char: no more fields need to create an Input object
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
// =============================================================================
//! \file
//! \copyright
// ================= end of file: systemarchitecture.hpp =======================
