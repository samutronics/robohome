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
public: inline metaTriStateOutput(cu16 sectionAddress);

public: virtual void next();

public: inline u16 extendedAddress() const;
public: inline void inputsDown(std::vector<u16>& down) const;
public: inline void inputsUp(std::vector<u16>& up) const;

private: inline u16 inputDownCount() const;
private: inline u16 inputUpCount() const;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline metaTriStateOutput::metaTriStateOutput(cu16 sectionAddress): metaOutput(sectionAddress) {}

inline u16 metaTriStateOutput::extendedAddress() const {
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the address property
			sizeof(u16) +
			// the size of the timeout ON property
			sizeof(u16) +
			// the size of the timeout OFF property
			sizeof(u16) +
			// the size of the input count property
			sizeof(u16) +
			// the number of connected UpDown input count property
			inputCount() * sizeof(u16);

	return read<u16>(address);
}

inline void metaTriStateOutput::inputsDown(std::vector<u16>& down) const {
	// The start address of inputsDown in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the address property
			sizeof(u16) +
			// the size of the timeout ON property
			sizeof(u16) +
			// the size of the timeout OFF property
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
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the address property
			sizeof(u16) +
			// the size of the timeout ON property
			sizeof(u16) +
			// the size of the timeout OFF property
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
	// The start address of inputDownCount in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the address property
			sizeof(u16) +
			// the size of the timeout ON property
			sizeof(u16) +
			// the size of the timeout OFF property
			sizeof(u16) +
			// the size of the input count property
			sizeof(u16) +
			// the number of connected UpDown input count property
			inputCount() * sizeof(u16) +
			// the size of the extendedAddress property
			sizeof(u16);

	return read<u16>(address);
}

inline u16 metaTriStateOutput::inputUpCount() const {
	// The start address of inputUpCount in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the address property
			sizeof(u16) +
			// the size of the timeout ON property
			sizeof(u16) +
			// the size of the timeout OFF property
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

	return read<u16>(address);
}

} // project
} // manager

#endif // _METATRISTATEOUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: metatristateoutput.hpp ===================
