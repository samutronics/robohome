//! =============================================================================
//! \copyright
//! \section		API documentation
//! \subsection
//! \brief			This module contains a hardware independent queue management
//!						system, that provide a solution to reach queues between
//!						tasks.
//! \author         Norbert Toth
//! \date			07.12.2014.
//! \details		Due to multithreading environment the system needs a commonly
//!						accessable module, that manage creation, query and store
//!						of ITCs.
//! \note			For abbreaviotion see section SDS
//! =============================================================================
#ifndef _IPCQUEUE_HPP_
#define _IPCQUEUE_HPP_

#include "../projectconfiguration.hpp"

namespace communication {

namespace ipc {

enum queueID {
	outboundQueue = 0,
	DECLARE_LAST_ENUM(queueID)
};

//! =============================================================================
//! \class		ipcQueue
//! \brief		This class manages the access to the registered queues from
//!					different thread.
//! \details	The threads can access the registered queues via queueID enum.
//!					Due to performance reasons usage of the template classes and
//!					methods prefered. To add new thread, the queueID enum has to
//!					be extended.
//! =============================================================================
class ipcQueue {
public: static inline ipcQueue& singleton();

public: inline xQueueHandle queue(queueID q);

private: ipcQueue();

private: xQueueHandle _q[configuration::ipcQueueCount];

private: static ipcQueue _singleton;
};

inline ipcQueue& ipcQueue::singleton() {
	return _singleton;
}

inline xQueueHandle ipcQueue::queue(queueID q) {
	return _q[q];
}

} // ipc
} //communication

#endif //_IPCQUEUE_HPP_

//! =============================================================================
//! \copyright
//! ======================= end of file: ipcQueue.cpp ===========================
