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
#include "inputmanager.hpp"
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
private: inline void LoadON() const;
private: inline void LoadOFF() const;

private: cu32							_dataByteCount;
private: std::vector<u32>				_data;
private: manager::input::InputManager*	_iputManager;

private: static input* _instance;
private: static xSemaphoreHandle _ISRQueue;
};

// =============================================================================
// Inline method implementation
// =============================================================================

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

inline void input::LoadON() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, 0x0);
}

inline void input::LoadOFF() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_PIN_0);
}

inline void input::IORead() {
	LoadOFF();
	for(u32 index = 0; index < _dataByteCount; index++) {
		SSIDataPut(SSI0_BASE, 0XFF);
		while(SSIBusy(SSI0_BASE)) {taskYIELD();}

		u32 tmp = 0;
		SSIDataGet(SSI0_BASE, &tmp);
		_data[(_dataByteCount / sizeof(_data[0])) - (index / sizeof(_data[0]))] |= (tmp & 0XFF) << ((_dataByteCount - 1 - (index % _dataByteCount)) * 8);
	}

	LoadON();
}

}  // inbound
}  // service

#endif //_INPUT_H_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: input.hpp =============================
