// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \note
// =============================================================================
#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "iservice.hpp"
#include "outputmanager.hpp"
#include "projectmanager.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace outbound {

class output: public IService {
private: output();
private: virtual void task(void *pvParameters);

private: void timerStart() const;
private: void IOStart() const;
private: inline void IOWrite() const;
private: inline void LoadON() const;
private: inline void LoadOFF() const;

private: cu32 _dataByteCount;
private: manager::output::OutputManager* _outputManager;

TO_BE_RUNABLE(output)
DEFINE_TH
};

inline void output::LoadON() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, GPIO_PIN_2);
}

inline void output::LoadOFF() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, 0x0);
}

inline void output::IOWrite() const {
	const std::vector<u32>& out = _outputManager->read();
	LoadOFF();
	for(u32 index = 0; index < _dataByteCount; index++) {
		SSIDataPut(SSI1_BASE, (out[(_dataByteCount / sizeof(out[0])) - (index / sizeof(out[0]))] >> ((_dataByteCount - 1 - (index % _dataByteCount)) * 8)) & 0XFF);
		while(SSIBusy(SSI1_BASE)) {taskYIELD();}
	}

	LoadON();
}

} // outbound
} // service

#endif // _OUTPUT_H_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: output.hpp ==========================
