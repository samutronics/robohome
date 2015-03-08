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
	EEPROMRead(&section, sizeof(u32), sizeof(section));
	return metaOutput(section & 0XFFFF);
}

inline metaTriStateOutput ProjectManager::triStateOutput() const {
	u32 section;
	EEPROMRead(&section, sizeof(u32), sizeof(section));

	u32 count[2];
	EEPROMRead(count, ((section >> 16) & 0xFFFF) - (((section >> 16) & 0xFFFF) % sizeof(count[0])), sizeof(count));

	u16 outputSection;
	if(2 < section % sizeof(count[0])) {
		outputSection = ((count[1] & 0XFF) << 8) | (count[0] >> ((section % sizeof(count[0])) * 8) & 0XFF);
	}
	else {
		outputSection = (count[0] >> ((section % sizeof(count[0])) * 8) & 0XFFFF);
	}

	return metaTriStateOutput(outputSection);
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
