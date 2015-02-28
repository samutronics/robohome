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
	triggerFallingEdge,
	triggerUnknown
};

class metaInput {
public: inline metaInput(const u16 sectionAddress, const u16 itemAddress);
public: inline u16 totalCount() const;
public: inline metaInput next() const;
public: inline TriggerType trigger() const;

private: const u16 _itemAddress;
private: const u16 _sectionAddress;
};

inline metaInput::metaInput(const u16 sectionAddress, const u16 itemAddress): _itemAddress(itemAddress), _sectionAddress(sectionAddress) {}

inline u16 metaInput::totalCount() const {
	static u32 count = std::numeric_limits<u32>::max();
	if(std::numeric_limits<u32>::max() == count) {
		EEPROMRead(&count, _sectionAddress / sizeof(u32), 1);
		count = ((count >> _sectionAddress % sizeof(u32) * 8) & 0xFFFF);
	}

	return static_cast<u16>(count);
}

inline TriggerType metaInput::trigger() const {
	u32 value;
	EEPROMRead(&value, _itemAddress / sizeof(u32), 1);
	return static_cast<TriggerType>(((value >> _itemAddress % sizeof(u32) * 8) & 0xFF));
}

inline metaInput metaInput::next() const {
	return metaInput(_sectionAddress, _itemAddress + 1);
}

} // project
} // manager

#endif // _METAINPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: metainput.hpp =======================
