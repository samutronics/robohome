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
#include "singletonfactory.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace output {

class OutputManager {
public: inline const std::map<u32, Output*>& outputs() const;
public: inline bool read(cu16 address) const;
public: inline const std::vector<u32>& read() const;

public: virtual ~OutputManager();

protected: inline OutputManager();

private: std::map<u32, Output*>	_output;
private: std::vector<u32>		_data;
};

typedef libs::SingletonFactory<OutputManager>	OutputManagerFactory;

// =============================================================================
// Inline method implementation
// =============================================================================

inline OutputManager::OutputManager(): _data(project::ProjectManager::getInstance()->output().count() + project::ProjectManager::getInstance()->triStateOutput().count()
		/ (sizeof(_data[0] * 8)) + ((project::ProjectManager::getInstance()->output().count() + project::ProjectManager::getInstance()->triStateOutput().count() % (sizeof(_data[0]) * 8)) ? 1 : 0), 0) {

	std::vector<u16> simpleTmp;
	project::metaOutput simpleoutput = project::ProjectManager::getInstance()->output();
	for(u32 index = 0; index < simpleoutput.count(); index++) {
		simpleoutput.inputs(simpleTmp);
		_output[simpleoutput.address()] = new Output(simpleoutput.address(), simpleoutput.timeoutON(), simpleoutput.timeoutOFF(), simpleTmp, _data);
		simpleTmp.clear();
		simpleoutput.next();
	}

	std::vector<u16> tristateTmpUp;
	std::vector<u16> tristateTmpDown;
	project::metaTriStateOutput tristateutput = project::ProjectManager::getInstance()->triStateOutput();
	for(u32 index = 0; index < tristateutput.count(); index++) {
		tristateutput.inputs(simpleTmp);
		tristateutput.inputsUp(tristateTmpUp);
		tristateutput.inputsDown(tristateTmpDown);
		_output[tristateutput.address()] = new TriStateOutput(
				tristateutput.address(),
				tristateutput.timeoutON(),
				tristateutput.timeoutOFF(),
				simpleTmp,
				_data,
				tristateutput.extendedAddress(),
				tristateTmpUp,
				tristateTmpDown);

		simpleTmp.clear();
		tristateTmpUp.clear();
		tristateTmpDown.clear();
		tristateutput.next();
	}
}

inline const std::map<u32, Output*>& OutputManager::outputs() const {
	return _output;
}

inline const std::vector<u32>& OutputManager::read() const {
	return _data;
}

inline bool OutputManager::read(cu16 address) const {
	return (_data[address / sizeof(_data[0])] & (1 << (address % sizeof(_data[0]))));
}

} // output
} // manager

#endif // _OUTPUTMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: outputmanager.hpp =====================
