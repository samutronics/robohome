// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METAOUTPUT_HPP_
#define _METAOUTPUT_HPP_

#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class metaOutput {
public: inline metaOutput(const u16 sectionAddress);
public: inline u16 address() const;
public: inline u16 timeout() const;
public: inline u16 totalCount() const;
public: inline void inputs(std::vector<u16>& input) const;

public: inline virtual void next();

protected: inline u16 inputCount() const;
protected: static inline void inputReader(std::vector<u16>& input, const u16 baseAddress, const u16 count);

protected: u16 _itemAddress;
protected: const u16 _sectionAddress;
};

inline metaOutput::metaOutput(const u16 sectionAddress): _itemAddress(sectionAddress + sizeof(u16)), _sectionAddress(sectionAddress) {}

inline u16 metaOutput::totalCount() const {
	static u32 count = std::numeric_limits<u32>::max();
	if(std::numeric_limits<u32>::max() == count) {
		EEPROMRead(&count, _sectionAddress / sizeof(u32), 1);
		count = ((count >> _sectionAddress % sizeof(u32) * 8) & 0xFFFF);
	}

	return static_cast<u16>(count);
}

inline u16 metaOutput::address() const {
	u32 value;
	// The address of address property in the EEPROM is directly at _itemAddress member.
	EEPROMRead(&value, _itemAddress / sizeof(u32), 1);
	return static_cast<u16>(((value >> _itemAddress % sizeof(u32) * 8) & 0xFFFF));
}

inline u16 metaOutput::timeout() const {
	u32 value;
	// The address of timeout property in the EEPROM is composed of:
	const u32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16);

	EEPROMRead(&value, (address) / sizeof(u32), 1);
	return static_cast<u16>(((value >> (address) % sizeof(u32) * 8) & 0xFFFF));
}

inline void metaOutput::inputs(std::vector<u16>& input) const {
	// The start address of inputs in the EEPROM is composed of:
	const u32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16) +
			// the size of the input count property
			sizeof(u16);

	inputReader(input, address, inputCount());
}

inline void metaOutput::next() {
	// the total item size in the EEPROM composed of:
	_itemAddress +=
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16) +
			// the size of the input count property
			sizeof(u16) +
			// the number of connected input count property
			inputCount() * sizeof(u16);
}

inline u16 metaOutput::inputCount() const {
	u32 value;
	// The address of input count property in the EEPROM is composed of:
	const u32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16);

	EEPROMRead(&value, address / sizeof(u32), 1);
	return static_cast<u16>(((value >> ((address % sizeof(u32)) * 8)) & 0xFFFF));
}

inline void metaOutput::inputReader(std::vector<u16>& input, const u16 baseAddress, const u16 count) {
	input.reserve(count);
	for(u32 item = 0; item < count; item++) {
		u32 value;
		const u32 address = baseAddress + item * sizeof(u16);
		EEPROMRead(&value, address / sizeof(u32), 1);
		input.push_back(static_cast<u16>((value >> ((address % sizeof(u32)) * 8)) & 0xFFFF));
	}
}

} // project
} // manager

#endif // _METAOUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: metaoutput.hpp =======================
