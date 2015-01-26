// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			07.12.2014.
//! \note
// =============================================================================
#include "ipcQueue.hpp"

using namespace communication::ipc;
using namespace service::sntp::configuration;
using namespace service::outbound::configuration;

ipcQueue ipcQueue::_singleton;

ipcQueue::ipcQueue() {
	_q[outboundQueue]	= xQueueCreate(outboundQueueLength, outboundQueueWidth);
}

// =============================================================================
//! \file
//! \copyright
// ======================== end of file: ipcqueue.cpp ==========================
