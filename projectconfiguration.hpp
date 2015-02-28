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
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string>
#include <vector>
#include <limits>

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
#include "emac.h"
#include "flash.h"
#include "timer.h"
#include "sysctl.h"
#include "eeprom.h"
#include "pin_map.h"
#include "interrupt.h"
#include "hibernate.h"

// =============================================================================
// networking related includes
// =============================================================================
#include "lwiplib.h"

// =============================================================================
// standard IO header
// =============================================================================
#include "uartstdio.h"

// =============================================================================
// Define types
// =============================================================================
typedef unsigned int	u32;
typedef int				s32;
typedef unsigned short	u16;
typedef short			s16;
typedef unsigned char	u8;
typedef char			s8;

typedef unsigned char*	up8;
typedef char*			sp8;

#define DECLARE_LAST_ENUM(enumName) last_##enumName##_element

// =============================================================================
// Configuration options
// =============================================================================
namespace systemGlobal {

const u32 requestedSystemClockFrequency = 120000000;
static u32 currentSystemClockFrequency = 0;
const u32 IPGatheringStrategie = IPADDR_USE_DHCP;

}  // system

namespace service {

namespace outbound {
namespace configuration {

const u32 outboundQueueLength = 4;
const u32 outboundQueueWidth = 1;

}  // configuration
}  // outbound

namespace inbound {
namespace configuration {

const u32 THQueueLength = 2;
const u32 THQueueWidth = 1;

}  // configuration
}  // inbound

namespace web {
namespace configuration {

const s8 defaultPage[] = "/index.htm";
const u16 port = 80;

}  // configuration
}  // web

namespace sntp {
namespace configuration {

const char url[] = "pool.ntp.org";
const u16 port = 123;
const u32 updatePeriode = 3600000;

}  // configuration
}  // sntp

namespace weather {
namespace configuration {

const char url[] = "api.openweathermap.org";
const u16 port = 80;
const u32 updatePeriode = 1800000; //0.5 h with the current scheduler setting: 1000 Hz task preemtion.

}  // configuration
}  // weather

namespace exosite {
namespace configuration {

const char url[] = "m2.exosite.com";
const u16 port = 80;
const u32 updatePeriode = 4000;

}  // configuration
}  // exosite

}  // manager

#endif // _PROJECTCONFIGURATION_HPP_
// =============================================================================
//! \file
//! \copyright
// ================= end of file: projectconfiguration.hpp =====================
