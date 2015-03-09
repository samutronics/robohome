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
	union {
		u32 read[sizeof(T) == sizeof(u8) ? sizeof(u8) : 2];
		u8 parse[sizeof(T) == sizeof(u8) ? (sizeof(u8) * 4) : 8];
	};

	EEPROMRead(read, address - (address % sizeof(read[0])), sizeof(read));

	T retVal = 0;
	for(u32 index = 0; index < sizeof(T); index++) {
		retVal |= (static_cast<T>(parse[index + (address % sizeof(read))]) << (index * 8));
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
