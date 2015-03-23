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
//! -# There is no separeted input for the services. Services act as an writer from the input manager point of view.
//! -# The project represented in binray form that will be processed by the system is valid and consistent.
//!
//!	\section projectmanagement Project management
//! The project is the human readable, text represented form that determines the run-time behavioral of the system related to its inputs and outputs.
//!
//!	\section ioarchitecture IO architecture
//!	The most important part of the system is the IO management. Logically, it's composed of two parts, the run-time model and the configuration management.
//! The run-time management covers the class and object model of run-time classes, properties and interactions with each other. The
//! configuration management determines how the compiled projects are interpreted and transformed into run-time objects.
//!
//! \subsection runtime Run-time model
//! There are two major requiremnts to the IO handling:
//! * The ablity to the IO reconfiguring both their properties and/or their numbers.
//! * The high performance in relation of the evaluation time.
//!
//! \subsubsection input Input
//! The main interaction of the inputs is the write operation on that from the direction of services. The main consumer
//! service is the Input that reads datas from the input boards and assigns it to the inputs. The communication is done
//! via SPI that's byte oriented.<br>
//! Due to performance considertion, the input handling is done via two main components, the Input and the InputManager.
//! Input objects only act as special kind of delegate that are responsible to evaluate input changes. There are two kind
//! of write operation on the InputManager:
//! * Write a single bit is needed by the services, like webservice.
//! * Write a mass data is required for the Input task.
//!
//! \subsubsection simpleoutput Simple output
//! This is the fundemantal output type that can control a lamp or other simple devices.<br>
//!
//! \subsubsection tristateoutput Tri-state output
//!
//!
//! \subsection configuration Project format
//! By a project the commissioner is able to change the behavioral of the system. Trough a project it's possible to the change
//! of parameters, that aren't lead directly to a system crash. These parameters could be e.g. the URL of ntp server, the number
//! inputs, connections between in and outputs.
//!
//! \subsubsection datastructure Data Structure
//! The main structural unit in the project is the section. The term section represents a "flat" collection of logilac related
//! properties. There is no restriction the properties to be the same types. Flat means in this context, that the sections can
//! be embedded into another, but in this case a section table has to be passed at the front of the new section level. It guarantees
//! the minimal effort to reach embedded sections.
//! A project represented in binary form is splitted up into four main sections:
//! * system configuration
//! * input properties
//! * output properties
//! * irrigation description
//! .
//! The project structure highly inspired by partitioning table used on the HDD's. So at the start of the project there will be the
//! offsets of sections listed above. That composed of the following internal structures:
//! * <b>System Configuration:</b> provides the following fields:
//!     * available hardware inputs. It has to be a number of multiple of 8 and represented on two bytes and depends on the available
//!			input modules in the system.
//!     * available hardware outputs. It has to be a number of multiple of 8 and represented on two bytes and depends on the available
//!			output modules in the system.
//!		* ID number (so called CIK) used by the exosite sevice.
//!		* IP configuration:
//!			* IP address on 4 byte, if it's zero, the system assume a DHCP strategie
//!			* subnet mask on 4 byte, considered only in case of static IP
//!			* default gateway on 4 byte, considered only in case of static IP
//!			.
//!		.
//! * <b>Input properties:</b> section contains the properties of inputs. The input section is a list of properties, so its's not
//! allowed to used and unused inputs follow each other. The currently supported input types:
//!		* BothEdges
//!		* RisingEdge
//!		* DeferredBothEdges
//!		* DeferredRisingEdge
//!		.
//! * <b>Output properties:</b> in the current implementation splitted up to two embedded subsections: first for the simple output and
//! the second describes the tri-state outputs. The output section consist of the following fields:
//!		* section table
//!		* simple output section:
//!			* number of simple outputs represented on unsigned short
//!			* properties of simple outputs:
//!				* address represented on unsigned short
//!				* turn-on time-out represented on unsigned short
//!				* turn-off time-out represented on unsigned short
//!				* number of connected inputs represented on unsigned short
//!				* lists of address of inputs represented on unsigned short
//!				.
//!			.
//!		* tri-state output section:
//!			* number of tri-state outputs represented on unsigned short
//!			* properties of tri-state outputs:
//!				* address represented on unsigned short
//!				* turn-on time-out represented on unsigned short
//!				* turn-off time-out represented on unsigned short
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
