// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "iservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace inbound {

class input {
public: static inline void deploy(void* pvParameters = NULL);
public: static void ISRHandler();
private: input();
private: void task(void *pvParameters);

private: void timerStart() const;
private: void IOStart() const;
private: inline void IORead();

private: u32 _timeSliceAccumulator;
private: cu32 _dataByteCount;
private: std::vector<u32> _data;
private: std::vector<u32> _dataAccumulator;

private: static input* _instance;
private: static xSemaphoreHandle _ISRQueue;
};

inline void input::deploy(void* pvParameters) {
	if(!_instance) {
		xSemaphoreHandle sync = NULL;
		if(!sync) {sync = xSemaphoreCreateMutex();}

		 xSemaphoreTake(sync, 0);
		if(!_instance) {
			_ISRQueue = xSemaphoreCreateBinary();
			_instance = new input();
		}

		xSemaphoreGive(sync);
	}

	_instance->task(pvParameters);
}

inline void input::IORead() {

}

}  // inbound
}  // service

#endif //_INPUT_H_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: input.hpp =============================
