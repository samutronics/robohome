// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METAOUTPUT_HPP_
#define _METAOUTPUT_HPP_

#include "abstractmetaio.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class metaOutput: public abstractMetaIO {
public: inline metaOutput(cu16 sectionAddress);
public: inline cu16 address() const;
public: inline cu16 timeoutOFF() const;
public: inline cu16 timeoutON() const;
public: inline void inputs(std::vector<u16>& input) const;

public: virtual void next();

protected: inline cu16 inputCount() const;
protected: static inline void inputReader(std::vector<u16>& input, cu16 baseAddress, cu16 count);
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline metaOutput::metaOutput(cu16 sectionAddress): abstractMetaIO(sectionAddress) {}

inline cu16 metaOutput::address() const {
	return read<u16>(_itemAddress);
}

inline cu16 metaOutput::timeoutOFF() const {
	// The address of timeout property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16);

	return read<u16>(address);
}

inline cu16 metaOutput::timeoutON() const {
	return 0;
}

inline void metaOutput::inputs(std::vector<u16>& input) const {
	// The start address of inputs in the EEPROM is composed of:
	cu32 address =
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

inline cu16 metaOutput::inputCount() const {
	// The address of input count property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16);

	return read<u16>(address);
}

inline void metaOutput::inputReader(std::vector<u16>& input, cu16 baseAddress, cu16 count) {
	input.reserve(count);
	for(u32 item = 0; item < count; item++) {
		cu32 address = baseAddress + item * sizeof(u16);
		input.push_back(read<u16>(address));
	}
}

} // project
} // manager

#endif // _METAOUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: metaoutput.hpp =======================
