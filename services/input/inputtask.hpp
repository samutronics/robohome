// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#ifndef _INPUTTASK_H_
#define _INPUTTASK_H_

#include "iinform.hpp"
#include "iservice.hpp"
#include "inputevaluator.hpp"
#include "singletonfactory.hpp"
#include "projectconfiguration.hpp"

namespace service {
namespace inbound {

class InputTask: public libs::IInform {
public: virtual ~InputTask();
public: void task(void *pvParameters);
public: inline void reset();
public: inline const std::vector<InputEvaluator*>& inputs() const;
public: inline void write(cu16 address, cu8 data, bool isBlocking = true);
public: inline void write(const std::vector<u8>& data);

public: virtual bool write(const libs::CommandsIterator& it);
public: virtual bool read(const libs::CommandsIterator& it, std::string& result) const;

protected: InputTask();

private: void timerStart() const;
private: void IOStart() const;
private: inline void IOTransmit(std::vector<u8>& data) const;
private: inline void LoadON() const;
private: inline void LoadOFF() const;

private: std::vector<InputEvaluator*>	_inputs;
private: std::vector<u32>				_dataChanged;
private: std::vector<u32>				_dataCurrent;
private: std::vector<u32>				_dataPrevious;
private: std::vector<u8>				_transmissionStatus;
private: std::vector<xSemaphoreHandle>	_lock;
DEFINE_TH
};

typedef libs::SingletonFactory<InputTask> InputTaskFactory;

// =============================================================================
// Inline method implementation
// =============================================================================

inline void InputTask::LoadON() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, 0x0);
}

inline void InputTask::LoadOFF() const {
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_PIN_0);
}

inline void InputTask::IOTransmit(std::vector<u8>& data) const {
	LoadOFF();
	for(u32 index = 0; index < data.size(); index++) {
		SSIDataPut(SSI0_BASE, 0XFF);
		while(SSIBusy(SSI0_BASE)) {taskYIELD();}

		u32 tmp = 0;
		SSIDataGet(SSI0_BASE, &tmp);
		data[data.size() - index - 1] |= (tmp & 0XFF);
	}

	LoadON();
}

inline void InputTask::reset() {
	for(u32 index = 0; index < _dataChanged.size(); _dataChanged[index++] = 0);
}

inline const std::vector<InputEvaluator*>& InputTask::inputs() const {
	return _inputs;
}

inline void InputTask::write(cu16 address, cu8 data, bool isBlocking) {
	cu32 bitCount = (sizeof(_dataChanged[0]) * 8);
	if(address > _dataChanged.size() * bitCount) {return;}

	xSemaphoreTake(_lock[address / bitCount], portMAX_DELAY);
	do {
		if(!(_dataChanged[address / bitCount] & (1 << address % bitCount))) {
			_dataPrevious[address / bitCount] = ((_dataPrevious[address / bitCount] & ~(1 << address % bitCount)) | (_dataCurrent[address / bitCount] & (1 << address % bitCount)));
			_dataCurrent[address / bitCount] = ((_dataCurrent[address / bitCount] & ~(1 << address % bitCount)) | ((data ? 1 : 0) << (address % bitCount)));
			_dataChanged[address / bitCount] |= (1 << address % bitCount);
			break;
		}

		taskYIELD();
	} while(isBlocking);

	xSemaphoreGive(_lock[address / bitCount]);
}

inline void InputTask::write(const std::vector<u8>& data) {
	for(u32 index = 0; index < data.size(); index++) {
		cu32 word = index / sizeof(_dataChanged[0]);
		xSemaphoreTake(_lock[word], portMAX_DELAY);

		cu32 byteMask = 0xFF << (index % sizeof(_dataChanged[0]) * 8);
		cu32 changeMask = ((~_dataChanged[word]) & byteMask);
		_dataPrevious[word] &= ~changeMask;
		_dataPrevious[word] |= (_dataCurrent[word] & changeMask);
		_dataCurrent[word] &= ~changeMask;
		_dataCurrent[word] |= ((data[index] << (index % sizeof(_dataCurrent[0]) * 8)) & changeMask);
		_dataChanged[word] |= byteMask;

		xSemaphoreGive(_lock[word]);
	}
}

}  // inbound
}  // service

#endif //_INPUTTASK_H_
// =============================================================================
//! \file
//! \copyright
// ====================== end of file: inputtask.hpp ===========================
