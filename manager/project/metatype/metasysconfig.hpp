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
	return read<u16>(_sectionAddress);
}

inline u16 metaSysConfig::hwOutputNumber() const {
	return read<u16>(_sectionAddress + sizeof(u16));
}

inline void metaSysConfig::cik(std::string& c) const {
	cu32 cikSize = read<u16>(_sectionAddress + sizeof(u16) + sizeof(u16) + sizeof(u32) + sizeof(u32) + sizeof(u32));
	for(u32 index = 0; index < cikSize; index++) {
		c += read<u8>(_sectionAddress + sizeof(u16) + sizeof(u16) + sizeof(u32) + sizeof(u32) + sizeof(u32) + sizeof(u16) + index);
	}
}

inline void metaSysConfig::network(u32& ipAddress, u32& netMask, u32& gateway) const {
	ipAddress = read<u32>(_sectionAddress + sizeof(u16) + sizeof(u16));
	netMask = read<u32>(_sectionAddress + sizeof(u16) + sizeof(u16) + sizeof(u32));
	gateway = read<u32>(_sectionAddress + sizeof(u16) + sizeof(u16) + sizeof(u32) + sizeof(u32));
}

} // project
} // manager

#endif // _METASYSCONFIG_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: metasysconfig.hpp =====================
