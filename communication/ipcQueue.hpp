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
#ifndef _IPCQUEUE_HPP_
#define _IPCQUEUE_HPP_

#include "../projectconfiguration.hpp"

namespace ipc {

enum queueID {
	outboundQueue = 0,
	DECLARE_LAST_ENUM(queueID)
};

class ipcQueue {

public: static inline ipcQueue& getIpcQueue();

//public: void registerQueue<queueID queue>(xQueueHandle queueHandler) {_p[queue] = queueHandler;}
//public: xQueueHandle queue<queueID queue>();

private: ipcQueue();

//private: xQueueHandle[ipc::configuration::ipcQueueCount] _q;
};

/*
template<> inline ipcQueue<>& ipcQueue<u32 queueCount>::getIpcQueue() {
	static ipcQueue<queueCount> q;
	return q;
}

template template<u32 queueCount> inline void ipcQueue<queueCount>::registerQueue<queueID queue>(xQueueHandle queueHandler) {
	_p[queue] = queueHandler;
}

inline xQueueHandle queueHandler queue<enum queue>() {
	retrurn _q[queue];
}
*/

} // ipc

#endif //_IPCQUEUE_HPP_

//! =============================================================================
//! \copyright
//! ======================= end of file: ipcQueue.cpp ===========================
