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
public: inline void name(std::string& n) const;

protected: u16 _itemAddress;
protected: static cu32 _nameSize = 16;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline abstractMetaIO::abstractMetaIO(const u16 sectionAddress): abstractMetaSection(sectionAddress), _itemAddress(_sectionAddress + sizeof(u16)) {}

inline void abstractMetaIO::reset() {
	_itemAddress = _sectionAddress + sizeof(u16);
}

inline u16 abstractMetaIO::count() const {
	return read<u16>(_sectionAddress);
}

inline void abstractMetaIO::name(std::string& n) const {
	for(u32 index = 0; index < _nameSize; index++) {
		n += read<u8>(_itemAddress + index);
	}
}

} // project
} // manager

#endif // _ABSTRACTMETAIO_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: abstractmetaio.hpp ====================
