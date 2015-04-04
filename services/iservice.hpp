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

// =============================================================================
//! \brief Definition of Top Halve part of THBH model
//!
//!	This macro provides implementation for the Top Halve interrupt handler. There
//! is a statical method definition for interrupt handling, and a queue for
//! communication the two halve.
// =============================================================================
#define DEFINE_TH												\
		private: static void handlerTH();						\
		private: static xSemaphoreHandle _THQueue;

// =============================================================================
//! \brief Declaration of the queue in the Top Halve part of THBH model
// =============================================================================
#define DECLARE_TH(derived)										\
		xSemaphoreHandle derived::_THQueue = NULL;

#endif //_IMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: iservice.h ===========================
