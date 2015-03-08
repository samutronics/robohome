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
protected: static inline void inputReader(std::vector<u16>& input, cu16 baseAddress, cu16 count);

protected: u16 _itemAddress;
protected: cu16 _sectionAddress;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline metaOutput::metaOutput(cu16 sectionAddress): _itemAddress(sectionAddress + sizeof(u16)), _sectionAddress(sectionAddress) {}

inline u16 metaOutput::totalCount() const {
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

inline u16 metaOutput::address() const {
	u32 count[2];
	EEPROMRead(count, _itemAddress - (_itemAddress % sizeof(count[0])), sizeof(count));

	u16 retVal;
	if(2 < _itemAddress % sizeof(count[0])) {
		retVal = ((count[1] & 0XFF) << 8) | (count[0] >> ((_itemAddress % sizeof(count[0])) * 8) & 0XFF);
	}
	else {
		retVal = (count[0] >> ((_itemAddress % sizeof(count[0])) * 8) & 0XFFFF);
	}

	return retVal;
}

inline u16 metaOutput::timeout() const {
	// The address of timeout property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16);

	u32 count[2];
	EEPROMRead(count, address - (address % sizeof(count[0])), sizeof(count));

	u16 retVal;
	if(2 < address % sizeof(count[0])) {
		retVal = ((count[1] & 0XFF) << 8) | (count[0] >> ((address % sizeof(count[0])) * 8) & 0XFF);
	}
	else {
		retVal = (count[0] >> ((address % sizeof(count[0])) * 8) & 0XFFFF);
	}

	return retVal;
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
	// The address of input count property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// the size of the address property
			sizeof(u16) +
			// the size of the time-out property
			sizeof(u16);

	u32 count[2];
	EEPROMRead(count, address - (address % sizeof(count[0])), sizeof(count));

	u16 retVal;
	if(2 < address % sizeof(count[0])) {
		retVal = ((count[1] & 0XFF) << 8) | (count[0] >> ((address % sizeof(count[0])) * 8) & 0XFF);
	}
	else {
		retVal = (count[0] >> ((address % sizeof(count[0])) * 8) & 0XFFFF);
	}

	return retVal;
}

inline void metaOutput::inputReader(std::vector<u16>& input, cu16 baseAddress, cu16 count) {
	input.reserve(count);
	for(u32 item = 0; item < count; item++) {
		u32 data[2];
		cu32 address = baseAddress + item * sizeof(u16);
		EEPROMRead(data, address - (address % sizeof(data[0])), sizeof(data));

		u16 in;
		if(2 < address % sizeof(data[0])) {
			in = ((data[1] & 0XFF) << 8) | (data[0] >> ((address % sizeof(data[0])) * 8) & 0XFF);
		}
		else {
			in = (data[0] >> ((address % sizeof(data[0])) * 8) & 0XFFFF);
		}

		input.push_back(in);
	}
}

} // project
} // manager

#endif // _METAOUTPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: metaoutput.hpp =======================
