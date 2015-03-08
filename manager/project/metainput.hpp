// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METAINPUT_HPP_
#define _METAINPUT_HPP_

#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

enum TriggerType {
	triggerBothEdges,
	triggerRisingEdge,
	triggerUnknown
};

class metaInput {
public: inline metaInput(const u16 sectionAddress);
public: inline u16 totalCount() const;
public: inline void next();
public: inline TriggerType trigger() const;

private: u16 _itemAddress;
private: cu16 _sectionAddress;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline metaInput::metaInput(cu16 sectionAddress): _itemAddress(sectionAddress + sizeof(u16)), _sectionAddress(sectionAddress) {}

inline u16 metaInput::totalCount() const {
	u32 count[2];
	EEPROMRead(count, _sectionAddress - (_sectionAddress % sizeof(count[0])), sizeof(count));
	return static_cast<u16>((((count[1] >> ((sizeof(count[1]) - _sectionAddress % sizeof(count[1]) - 1) * 8)) & 0XFF) << 8) |
			((count[0] >> (_sectionAddress % sizeof(count[0]) * 8)) & 0XFF));
}

inline TriggerType metaInput::trigger() const {
	u32 value;
	EEPROMRead(&value, _itemAddress - (_itemAddress % sizeof(value)), sizeof(value));
	return static_cast<TriggerType>((value >> (_itemAddress % sizeof(value) * 8)) & 0XFF);
}

inline void metaInput::next() {
	_itemAddress += sizeof(u8);
}

} // project
} // manager

#endif // _METAINPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: metainput.hpp =======================
