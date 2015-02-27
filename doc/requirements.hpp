// =============================================================================
//! \file
//! \brief			Documentation page
//! \author         Norbert Toth
//! \date			21.02.2015.
//! \note
// =============================================================================
//!
//! \page requirements Requirements
//! \tableofcontents
//! \section openissue Open issues
//!
//! -# The client wants to manipulate same output from multiple input. What is the prefered evaluation
//! order if multiple changes occured at same time?
//! -# The client preferes the "last win" evaluation strategie in case of tri-state outputs. It could be dangerous,
//! becasue the IO read / write is done via polling. Data loss can be occured.
//! -# The client wants to make a logical binding betwen the irrigation and IO management. Suggestion: it should be
//! implemented in different component.
//!
//! \section userstory User stories
//! -# Project related user stories:
//!     -# I, as a commissioner, would like to have a project structure that holds various configuration options.
//!     -# I, as a commissioner, would like to configure the IP address gathering and the IP address itself in the project.
//!     -# I, as a commissioner, would like to define the number of inputs and their properties in the project.
//!     -# I, as a commissioner, would like to define the number of outputs and their properties in the project.
//!     -# I, as a commissioner, would like to define connections between the inputs and outputs.
//! -# Hardware input related user stories:
//!     -# I, as a commissioner would like an input to reacts for both of 0->1 and 1->0 changes.
//!     -# I, as a commissioner would like an input to reacts only for the 0->1 input change.
//! -# Hardware output related user stories:
//!     -# I, as a commissioner would like to configure the output to changes the state of one specific output bit immeditly.
//!     The initial state should be the inactive.
//!     -# I, as a commissioner would like to configure the output to turn on the state of one specific output bit immeditly
//!     and turn off with time-out. The initial state should be the inactive and the time-out is adjustable during commissioning.
//!     -# I, as a commissioner would like to configure the output to turn on the state of one specific output bit with time-out
//!     and turn off inmediatly. The initial state should be the inactive and the time-out is adjustable during commissioning.
//!     -# I, as a commissioner would like to define groups of outputs that are controlled by one specific input.
//!     -# I, as a commissioner would like to configure an output that's controlled by many inputs.
//!     -# I, as a commissioner would like to have an output that controls two bits with the states:
//!         * both turned off
//!         * first turned on, second turned off
//!         * first turned off, second turned on
//!         .
//!     and between the passive -> active transition a delay should be applied.
//!
//! \section requirement Requirements
//!
//! * <b>Get weather informations:</b> The device should query weather informations from the http://openweathermap.org
//! * <b>Supported input types:</b>
//!     * simple input: stateless, reacts on changes
//!     * push button: react only on 0->1 change
//! * <b>Supported output types:</b>
//!     * simple timed:
//!         * deferred state change possibility
//!         * configurable time-out
//!         * maximal time-out: 2^16*0.1 ~ 4.55 days
//!     * tri state timed:
//!         * 2 outputs with exclusive OR
//!         * state change delayed 500ms
//!
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: requirements.hpp =========================
