// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METATRISTATEOUTPUT_HPP_
#define _METATRISTATEOUTPUT_HPP_

#include "metaoutput.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class metaTriStateOutput: public metaOutput {
public: inline metaTriStateOutput(const u16 sectionAddress);

public: inline virtual void next();

public: inline u16 extendedAddress() const;
public: inline void inputsDown(std::vector<u16>& down) const;
public: inline void inputsUp(std::vector<u16>& up) const;

private: inline u16 inputDownCount() const;
private: inline u16 inputUpCount() const;
};

inline metaTriStateOutput::metaTriStateOutput(const u16 sectionAddress): metaOutput(sectionAddress) {}

inline void metaTriStateOutput::next() {
	// the total item size in the EEPROM composed of:
	_itemAddress +=
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16) +
			// the size of the UpDown input count property
			sizeof(u16) +
			// the number of connected UpDown input count property
			inputCount() * sizeof(u16) +
			// the size of the extendedAddress property
			sizeof(u16) +
			// the size of the Down input count property
			sizeof(u16) +
			// the number of connected Down input count property
			inputDownCount() * sizeof(u16) +
			// the size of the Up input count property
			sizeof(u16) +
			// the number of connected Up input count property
			inputUpCount() * sizeof(u16);
}

inline u16 metaTriStateOutput::extendedAddress() const {
	u32 value;
	// The start address of extendedAddress in the EEPROM is composed of:
	const u32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16) +
			// the size of the input count property
			sizeof(u16) +
			// the number of connected UpDown input count property
			inputCount() * sizeof(u16);

	EEPROMRead(&value, address / sizeof(u32), 1);
	return static_cast<u16>(((value >> address % sizeof(u32) * 8) & 0xFFFF));
}

inline void metaTriStateOutput::inputsDown(std::vector<u16>& down) const {
	// The start address of inputsDown in the EEPROM is composed of:
	const u32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16) +
			// the size of the input count property
			sizeof(u16) +
			// the number of connected UpDown input count property
			inputCount() * sizeof(u16) +
			// the size of the extendedAddress property
			sizeof(u16) +
			// the size of the Down input count property
			sizeof(u16);

	inputReader(down, address, inputDownCount());
}

inline void metaTriStateOutput::inputsUp(std::vector<u16>& up) const {
	// The start address of inputsUp in the EEPROM is composed of:
	const u32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16) +
			// the size of the UpDown input count property
			sizeof(u16) +
			// the number of connected UpDown input count property
			inputCount() * sizeof(u16) +
			// the size of the extendedAddress property
			sizeof(u16) +
			// the size of the Down input count property
			sizeof(u16) +
			// the number of connected Down input count property
			inputDownCount() * sizeof(u16) +
			// the size of the Up input count property
			sizeof(u16);

	inputReader(up, address, inputUpCount());
}

inline u16 metaTriStateOutput::inputDownCount() const {
	u32 value;
	// The start address of inputDownCount in the EEPROM is composed of:
	const u32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16) +
			// the size of the input count property
			sizeof(u16) +
			// the number of connected UpDown input count property
			inputCount() * sizeof(u16) +
			// the size of the extendedAddress property
			sizeof(u16);

	EEPROMRead(&value, address / sizeof(u32), 1);
	return static_cast<u16>(((value >> address % sizeof(u32) * 8) & 0xFFFF));
}

inline u16 metaTriStateOutput::inputUpCount() const {
	u32 value;
	// The start address of inputUpCount in the EEPROM is composed of:
	const u32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16) +
			// the size of the UpDown input count property
			sizeof(u16) +
			// the number of connected UpDown input count property
			inputCount() * sizeof(u16) +
			// the size of the extendedAddress property
			sizeof(u16) +
			// the size of the Down input count property
			sizeof(u16) +
			// the number of connected Down input count property
			inputDownCount() * sizeof(u16);

	EEPROMRead(&value, address / sizeof(u32), 1);
	return static_cast<u16>(((value >> address % sizeof(u32) * 8) & 0xFFFF));
}

} // project
} // manager

#endif // _METATRISTATEOUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: metatristateoutput.hpp ===================
