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

namespace manager {

class IManager {
	protected: virtual void task(void *pvParameters) = 0;
};

}  // namespace manager

#define TO_BE_RUNABLE(derived)									\
	public: static inline void start(void *pvParameters) {		\
		static derived d;										\
		d.task(pvParameters);									\
		while(1);												\
	};															\

#define DEFINE_TH												\
		private: static void handlerTH();						\
		private: static xQueueHandle _THQueue;

#define DECLARE_TH(derived)										\
		xQueueHandle derived::_THQueue;

#endif //_IMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: imanager.h ============================
