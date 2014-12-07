//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			04.12.2014.
//! \details
//! \note
//! =============================================================================
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
#include "sysctl.h"
#include "interrupt.h"

// =============================================================================
// Define types
// =============================================================================
typedef unsigned int	u32;
typedef int				s32;

// =============================================================================
// Configuration options
// =============================================================================
namespace manager {

namespace configuration {

namespace outboundTask {

const u32 outboundQueueLength = 4;
const u32 outboundQueueWidth = 1;

}  // namespace inputTask

}  // namespace configuration

}  // namespace manager



#endif // _PROJECTCONFIGURATION_HPP_
//! =============================================================================
//! \copyright
//! ================= end of file: projectconfiguration.hpp =====================
