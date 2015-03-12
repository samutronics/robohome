// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METASYSCONFIG_HPP_
#define _METASYSCONFIG_HPP_

#include "abstractmetasection.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class metaSysConfig: public abstractMetaSection {
public: inline metaSysConfig(cu16 sectionAddress);
public: inline u16 hwInputNumber() const;
public: inline u16 hwOutputNumber() const;
public: inline void cik(std::string& c) const;
public: inline void network(u32& ipAddress, u32& netMask, u32& gateway) const;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline metaSysConfig::metaSysConfig(cu16 sectionAddress): abstractMetaSection(sectionAddress) {}

inline u16 metaSysConfig::hwInputNumber() const {
	return 16; // hard-wired yet. Implementation comes later.
}

inline u16 metaSysConfig::hwOutputNumber() const {
	return 16; // hard-wired yet. Implementation comes later.
}

inline void metaSysConfig::cik(std::string& c) const {
	u32 tmp[2];
	EEPROMRead(tmp, _sectionAddress - (_sectionAddress % sizeof(tmp[0])), sizeof(tmp));

	u32 cikSize;
	if(2 < _sectionAddress % sizeof(tmp[0])) {
		cikSize = ((tmp[1] & 0XFF) << 8) | (tmp[0] >> ((_sectionAddress % sizeof(tmp[0])) * 8) & 0XFF);
	}
	else {
		cikSize = (tmp[0] >> ((_sectionAddress % sizeof(tmp[0])) * 8) & 0XFFFF);
	}

	for(u32 index = 0; index < cikSize; index++) {
		u32 character;
		EEPROMRead(&character, (_sectionAddress + sizeof(u16) + index) - ((_sectionAddress + sizeof(u16) + index) % sizeof(character)), sizeof(character));
		c += ((character >> (((_sectionAddress + sizeof(u16) + index) % sizeof(character)) * 8)) & 0XFF);
	}
}

} // project
} // manager

#endif // _METASYSCONFIG_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: metasysconfig.hpp =====================
