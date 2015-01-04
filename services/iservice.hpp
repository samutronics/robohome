// =============================================================================
//! \file
//! \brief			Task definition interface file
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \details		This file define the interface, and the macros for the
//!						implementation, that are used for TaaT.
//! \note			For abbrevation see SDS
// =============================================================================
#ifndef _IMANAGER_H_
#define _IMANAGER_H_

namespace service {

// =============================================================================
//! \brief Interface for tasks
//!
//!	The interface provide the method, that is harmed to the requirement of FreeRTOS
//! threads
// =============================================================================
class IService {
	protected: virtual void task(void *pvParameters) = 0;
};

}  // service

// =============================================================================
//! \brief C source capable thread implementation for wrapping the interface implementations
//!
//!	Due to C sources cannot work on C++ obejct, C++ statis method needs to
//! wrap the thread implementation. Furthermore, only one thread is required and
//! allowed per type. To fulfill that, a special singleton design pattern is applied.
// =============================================================================
#define TO_BE_RUNABLE(derived)									\
	public: static inline void start(void *pvParameters) {		\
		static derived d;										\
		d.task(pvParameters);									\
		while(1);												\
	};															\

// =============================================================================
//! \brief Definition of Top Halve part of THBH model
//!
//!	This macro provides implementation for the Top Halve interrupt handler. There
//! is a statical method definition for interrupt handling, and a queue for
//! communication the two halve.
// =============================================================================
#define DEFINE_TH												\
		private: static void handlerTH();						\
		private: static xQueueHandle _THQueue;

// =============================================================================
//! \brief Declaration of the queue in the Top Halve part of THBH model
// =============================================================================
#define DECLARE_TH(derived)										\
		xQueueHandle derived::_THQueue;

#endif //_IMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: iservice.h ===========================
