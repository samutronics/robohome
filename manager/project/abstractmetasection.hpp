// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.03.2015.
//! \note
// =============================================================================
#ifndef _ABSTRACTMETASECTION_HPP_
#define _ABSTRACTMETASECTION_HPP_

#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class abstractMetaSection {
protected: inline abstractMetaSection(const u16 sectionAddress);

protected: template<typename T> static T read(cu16 address);
protected: cu16 _sectionAddress;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline abstractMetaSection::abstractMetaSection(const u16 sectionAddress): _sectionAddress(sectionAddress) {}

template<typename T> T abstractMetaSection::read(cu16 address) {
	union buffer {
		u32 read[2];
		u8 parse[sizeof(read)];
	} b;

	EEPROMRead(b.read, address - (address % sizeof(b.read[0])), sizeof(b.read));

	T retVal;
	for(u32 index = address % sizeof(b.read); index < address % sizeof(b.read) + sizeof(T); index++) {
		retVal |= =b.parse[index] << (index * 8));
	}

	return retVal;
}

} // project
} // manager

#endif // _ABSTRACTMETASECTION_HPP_
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: abstractmetasection.hpp ==================
