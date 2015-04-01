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
#include "singletonfactory.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace inbound {

class input {
protected: input();

public: void task(void *pvParameters);
private: void timerStart() const;
private: void IOStart() const;
private: inline void IORead();
private: inline void LoadON() const;
private: inline void LoadOFF() const;

private: std::vector<u8> _data;
DEFINE_TH
};

typedef libs::SingletonFactory<input> InputFactory;

// =============================================================================
// Inline method implementation
// =============================================================================

inline void input::LoadON() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, 0x0);
}

inline void input::LoadOFF() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_PIN_0);
}

inline void input::IORead() {
	LoadOFF();
	for(u32 index = 0; index < _data.size(); index++) {
		SSIDataPut(SSI0_BASE, 0XFF);
		while(SSIBusy(SSI0_BASE)) {taskYIELD();}

		u32 tmp = 0;
		SSIDataGet(SSI0_BASE, &tmp);
		_data[_data.size() - index - 1] |= (tmp & 0XFF) << ((_data.size() - (index % _data.size()) - 1) * 8);
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
