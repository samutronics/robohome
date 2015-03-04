// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#ifndef _OUTPUTMANAGER_HPP_
#define _OUTPUTMANAGER_HPP_

#include "output.hpp"
#include "tristateoutput.hpp"
#include "projectmanager.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace output {

class OutputManager {
public: static inline OutputManager* getInstance();

public: const std::vector<Output*>& outputs() const;

public: bool read(cu16 address) const;
public: const std::vector<u32>& read() const;

public: virtual ~OutputManager();
private: inline OutputManager();

private: std::vector<Output*>			_output;
private: std::vector<u32>				_data;

private: static OutputManager* _instance;
};

inline OutputManager* OutputManager::getInstance() {
	if(!_instance) {
		xSemaphoreHandle sync = NULL;
		if(!sync) {
			sync = xSemaphoreCreateMutex();
		}

		 xSemaphoreTake(sync, 0);
		if(!_instance) {
			_instance = new OutputManager();
		}

		xSemaphoreGive(sync);
	}

	return _instance;
}

inline OutputManager::OutputManager():
				_output(project::ProjectManager::getInstance()->output().totalCount() + project::ProjectManager::getInstance()->triStateOutput().totalCount()),
				_data(_output.size() / sizeof(_data[0]) + (_output.size() % (sizeof(_data[0]) ? 1 : 0))) {
	std::vector<u16> simpleTmp(100);
	project::metaOutput simpleoutput = project::ProjectManager::getInstance()->output();
	for(u32 index = 0; index < simpleoutput.totalCount(); index++) {
		simpleoutput.inputs(simpleTmp);
		_output.push_back(new Output(simpleoutput.address(), simpleoutput.timeout(), simpleTmp, _data));
	}

	std::vector<u16> tristateTmpUp(100);
	std::vector<u16> tristateTmpDown(100);
	project::metaTriStateOutput tristateutput = project::ProjectManager::getInstance()->triStateOutput();
	for(u32 index = 0; index < tristateutput.totalCount(); index++) {
		tristateutput.inputs(simpleTmp);
		tristateutput.inputsUp(tristateTmpUp);
		tristateutput.inputsDown(tristateTmpDown);
		_output.push_back(new TriStateOutput(tristateutput.address(), tristateutput.timeout(), simpleTmp, _data, tristateutput.extendedAddress(), tristateTmpUp, tristateTmpDown));
	}
}

const std::vector<Output*>& OutputManager::outputs() const {
	return _output;
}

const std::vector<u32>& OutputManager::read() const {
	return _data;
}

bool OutputManager::read(cu16 address) const {
	return (_data[address / sizeof(_data[0])] & (1 << (address % sizeof(_data[0]))));
}

} // output
} // manager

#endif // _OUTPUTMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: outputmanager.hpp =====================
