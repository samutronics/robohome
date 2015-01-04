// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			07.12.2014.
//! \note
// =============================================================================
#include "ipcQueue.hpp"

using namespace communication::ipc;
using namespace service::sntpTask::configuration;
using namespace service::outboundTask::configuration;

ipcQueue ipcQueue::_singleton;

ipcQueue::ipcQueue() {
	_q[outboundQueue]	= xQueueCreate(outboundQueueLength, outboundQueueWidth);
	_q[rtcQueue]		= xQueueCreate(rtcQueueLength, rtcQueueWidth);
}

// =============================================================================
//! \file
//! \copyright
// ======================== end of file: ipcqueue.cpp ==========================
