//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			07.12.2014.
//! \details
//! \note
//! =============================================================================
#include "ipcQueue.hpp"

using namespace communication::ipc;
using namespace manager::rtcTask::configuration;
using namespace manager::outboundTask::configuration;

ipcQueue ipcQueue::_singleton;

ipcQueue::ipcQueue() {
	_q[outboundQueue]	= xQueueCreate(outboundQueueLength, outboundQueueWidth);
	_q[rtcQueue]		= xQueueCreate(rtcQueueLength, rtcQueueWidth);
}

//! =============================================================================
//! \copyright
//! ======================== end of file: ipcQueue.cpp ==========================
