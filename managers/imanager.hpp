//! =============================================================================
//! \copyright
//! \section
//! \subsection
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \details
//! \note
//! =============================================================================
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
//! =============================================================================
//! \copyright
//! ========================= end of file: imanager.h ============================
