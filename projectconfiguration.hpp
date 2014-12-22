// =============================================================================
//! \file
//! \brief			This file contains the core settings, that are relating to
//!					the project, but not to the hardware or the operation system.
//! \author         Norbert Toth
//! \date			04.12.2014.
//! \details		This file contains the project wide required stuffs, like
//!						configuration constants, includes and definitions. The
//!						configuration section is breaked down into several
//!						namespaces. The namespace convenctions are follows
//!						the follovings:
//!						* the outermost namespace is equal to the module
//!							collector namespace (the directory, where
//!							the module lives)
//!						* the midle namespace is equal to the module namespace
//!						* whitin the namespace above is the namespace "configuration"
//! \note
//!	\warning		Change this settings could highly affects the system
//!						performance and / or memory consuption
// =============================================================================
#ifndef _PROJECTCONFIGURATION_HPP_
#define _PROJECTCONFIGURATION_HPP_

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
// Hardware dependent includes
// =============================================================================
#include "hw_gpio.h"
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"

// =============================================================================
// Drived librarie includes
// =============================================================================
#include "gpio.h"
#include "flash.h"
#include "timer.h"
#include "sysctl.h"
#include "pin_map.h"
#include "interrupt.h"
#include "hibernate.h"

// =============================================================================
// networking related includes
// =============================================================================
#include "lwiplib.h"

// =============================================================================
// Define types
// =============================================================================
typedef unsigned int	u32;
typedef int				s32;
typedef unsigned short	u16;
typedef short			s16;
typedef unsigned char	u8;
typedef char			s8;

#define DECLARE_LAST_ENUM(enumName) last_##enumName##_element

// =============================================================================
// Configuration options
// =============================================================================
namespace system {

const u32 requestedSystemClockFrequency = 120000000;
static u32 currentSystemClockFrequency = 0;

}  // system

namespace manager {

namespace outboundTask {
namespace configuration {

const u32 outboundQueueLength = 4;
const u32 outboundQueueWidth = 1;

}  // configuration
}  // outboundTask

namespace inboundTask {
namespace configuration {

const u32 THQueueLength = 2;
const u32 THQueueWidth = 1;

}  // configuration
}  // inboundTask

namespace networkTask {
namespace configuration {

const u32 IPGatheringStrategie = IPADDR_USE_DHCP;

}  // configuration
}  // inboundTask

namespace rtcTask {

namespace configuration {

const u32 rtcQueueLength = 1;
const u32 rtcQueueWidth = sizeof(u32);

}  // configuration
}  // inboundTask

}  // manager

#endif // _PROJECTCONFIGURATION_HPP_
// =============================================================================
//! \file
//! \copyright
// ================= end of file: projectconfiguration.hpp =====================
