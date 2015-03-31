// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _PROJECTMANAGER_HPP_
#define _PROJECTMANAGER_HPP_

#include "iinform.hpp"
#include "metainput.hpp"
#include "metaoutput.hpp"
#include "metasysconfig.hpp"
#include "metairrigation.hpp"
#include "singletonfactory.hpp"
#include "commandsiterator.hpp"
#include "metatristateoutput.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class ProjectManager: public libs::IInform {
public: void read(std::vector<u32>& project) const;
public: bool write(std::vector<u32>& project) const;

public: virtual bool write(const libs::CommandsIterator& it);
public: virtual bool read(const libs::CommandsIterator& it, std::string& result) const;

public: void trace() const;
public: void parse() const;

public: inline metaInput			input()				const;
public: inline metaOutput			output()			const;
public: inline metaTriStateOutput	triStateOutput()	const;
public: inline metaSysConfig		sysConfig()			const;
public: inline metaIrrigation		irrigation()		const;

protected: inline ProjectManager();
};

typedef libs::SingletonFactory<ProjectManager>	ProjectManagerFactory;

// =============================================================================
// Inline method implementation
// =============================================================================

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
	return metaOutput((section + sizeof(u32)) & 0XFFFF);
}

inline metaTriStateOutput ProjectManager::triStateOutput() const {
	u32 section;
	EEPROMRead(&section, sizeof(u32), sizeof(section));
	return metaTriStateOutput(abstractMetaSection::read<u16>(section & 0xFFFF) + sizeof(u32) + (section & 0XFFFF));
}

inline metaIrrigation ProjectManager::irrigation() const {
	u32 section;
	EEPROMRead(&section, sizeof(u32), sizeof(section));
	return metaIrrigation(static_cast<u16>((section >> 16) & 0xFFFF));
}

} // project
} // manager

#endif // _PROJECTMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: projectmanager.hpp =====================
