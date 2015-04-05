// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \note
// =============================================================================
#ifndef _OUTPUTTASK_HPP_
#define _OUTPUTTASK_HPP_

#include "iinform.hpp"
#include "iservice.hpp"
#include "projectmanager.hpp"
#include "outputevaluator.hpp"
#include "commandsiterator.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace outbound {

class OutputTask: public libs::IInform {
public: virtual ~OutputTask();

public: void task(void *pvParameters);

public: virtual bool write(const libs::CommandsIterator& it);
public: virtual bool read(const libs::CommandsIterator& it, std::string& result) const;

protected: OutputTask();

private: void timerStart() const;
private: void IOStart() const;
private: inline void IOTransmit() const;
private: inline void LoadON() const;
private: inline void LoadOFF() const;
private: inline void LatchON() const;
private: inline void LatchOFF() const;

private: cu32 _dataByteCount;

private: std::map<u32, OutputEvaluator*>	_output;
private: std::vector<u32>					_data;

DEFINE_TH
};

typedef libs::SingletonFactory<OutputTask> OutputTaskFactory;

// =============================================================================
// Inline method implementation
// =============================================================================

inline void OutputTask::LoadON() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, GPIO_PIN_2);
}

inline void OutputTask::LoadOFF() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_2, 0x0);
}

inline void OutputTask::LatchON() const {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
}

inline void OutputTask::LatchOFF() const {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);
}

inline void OutputTask::IOTransmit() const {
	LatchOFF();
	for(u32 index = 0; index < _dataByteCount; index++) {
		SSIDataPut(SSI1_BASE, (_data[(_dataByteCount / sizeof(_data[0])) - (index / sizeof(_data[0]))] >> ((_dataByteCount - 1 - (index % _dataByteCount)) * 8)) & 0XFF);
		while(SSIBusy(SSI1_BASE)) {taskYIELD();}
	}

	LatchON();
}

} // outbound
} // service

#endif // _OUTPUTTASK_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: outputtask.hpp ========================
