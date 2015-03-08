// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _PROJECTMANAGER_HPP_
#define _PROJECTMANAGER_HPP_

#include "metainput.hpp"
#include "metaoutput.hpp"
#include "metasysconfig.hpp"
#include "metairrigation.hpp"
#include "metatristateoutput.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class ProjectManager {
public: static inline ProjectManager* getInstance();

public: void read(std::vector<u32>& project) const;
public: bool write(std::vector<u32>& project) const;

public: void trace() const;
public: void parse() const;

public: inline metaInput			input()				const;
public: inline metaOutput			output()			const;
public: inline metaTriStateOutput	triStateOutput()	const;
public: inline metaSysConfig		sysConfig()			const;
public: inline metaIrrigation		irrigation()		const;

private: inline ProjectManager();

private: static ProjectManager* _instance;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline ProjectManager* ProjectManager::getInstance() {
	if(!_instance) {
		xSemaphoreHandle sync = NULL;
		if(!sync) {
			sync = xSemaphoreCreateMutex();
		}

		 xSemaphoreTake(sync, 0);
		if(!_instance) {
			_instance = new ProjectManager();
		}

		xSemaphoreGive(sync);
	}

	return _instance;
}

inline ProjectManager::ProjectManager() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	EEPROMInit();
}

inline metaSysConfig ProjectManager::sysConfig() const {
	u32 section;
	EEPROMRead(&section, 0, sizeof(section));
	return metaSysConfig(static_cast<u16>(section & 0xFFFF));
}

inline metaInput ProjectManager::input() const {
	u32 section;
	EEPROMRead(&section, 0, sizeof(section));
	return metaInput(static_cast<u16>((section >> 16) & 0xFFFF));
}

inline metaOutput ProjectManager::output() const {
	u32 section;
	EEPROMRead(&section, 1, 1);
	EEPROMRead(&section, ((section & 0xFFFF) / sizeof(u32)), 1);
	return metaOutput(static_cast<u16>(section >> (((section & 0xFFFF) % sizeof(u32)) * 8) & 0xFFFF));
}

inline metaTriStateOutput ProjectManager::triStateOutput() const {
	u32 section;
	EEPROMRead(&section, 1, 1);
	EEPROMRead(&section, (((section >> 16) & 0xFFFF) / sizeof(u32)), 1);
	return metaTriStateOutput(static_cast<u16>(section >> (((section & 0xFFFF) % sizeof(u32)) * 8) & 0xFFFF));
}

inline metaIrrigation ProjectManager::irrigation() const {
	u32 section;
	EEPROMRead(&section, 1, 1);
	return metaIrrigation(0);
}

} // project
} // manager

#endif // _PROJECTMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: projectmanager.hpp =====================
