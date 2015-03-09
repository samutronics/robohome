// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.03.2015.
//! \note
// =============================================================================
#ifndef _ABSTRACTMETAIO_HPP_
#define _ABSTRACTMETAIO_HPP_

#include "abstractmetasection.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class abstractMetaIO: public abstractMetaSection {
protected: inline abstractMetaIO(const u16 sectionAddress);

public: virtual void next() = 0;
public: inline void reset();
public: inline u16 count() const;

protected: u16 _itemAddress;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline abstractMetaIO::abstractMetaIO(const u16 sectionAddress): abstractMetaSection(sectionAddress), _itemAddress(_sectionAddress + sizeof(u16)) {}

inline void abstractMetaIO::reset() {
	_itemAddress = _sectionAddress + sizeof(u16);
}

inline u16 abstractMetaIO::count() const {
	u32 count[2];
	EEPROMRead(count, _sectionAddress - (_sectionAddress % sizeof(count[0])), sizeof(count));

	u16 retVal;
	if(2 < _sectionAddress % sizeof(count[0])) {
		retVal = ((count[1] & 0XFF) << 8) | (count[0] >> ((_sectionAddress % sizeof(count[0])) * 8) & 0XFF);
	}
	else {
		retVal = (count[0] >> ((_sectionAddress % sizeof(count[0])) * 8) & 0XFFFF);
	}

	return retVal;
}

} // project
} // manager

#endif // _ABSTRACTMETAIO_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: abstractmetaio.hpp ====================
