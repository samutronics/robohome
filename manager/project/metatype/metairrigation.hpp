// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METAIRRIGATION_HPP_
#define _METAIRRIGATION_HPP_

#include "abstractmetaio.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

enum CircleMode {
	Normal,
	Grown
};

class metaIrrigation: public abstractMetaIO {
public: inline metaIrrigation(cu16 sectionAddress);

public: virtual void next();
public: inline cu16 area() const;
public: inline cu16 input() const;
public: inline cu16 upTime() const;
public: inline cu16 startTime() const;
public: inline CircleMode mode() const;
public: inline cu16 offsetTime() const;
public: inline cu16 dailyPrecipitation() const;
public: inline cu16 unitPrecipitation() const;
public: inline cu16 repeatCount() const;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline metaIrrigation::metaIrrigation(cu16 sectionAddress): abstractMetaIO(sectionAddress) {}

inline cu16 metaIrrigation::area() const {
	return read<u16>(_itemAddress + _nameSize);
}

inline cu16 metaIrrigation::input() const {
	// The address targetted input property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the area property
			sizeof(u16);

	return read<u16>(address);
}

inline cu16 metaIrrigation::upTime() const {
	// The address upTime property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the area property
			sizeof(u16) +
			// the size of the input property
			sizeof(u16);

	return read<u16>(address);
}

inline cu16 metaIrrigation::startTime() const {
	// The address startTime property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the area property
			sizeof(u16) +
			// the size of the input property
			sizeof(u16) +
			// the size of the upTime property
			sizeof(u16);

	return read<u16>(address);
}

inline CircleMode metaIrrigation::mode() const {
	// The address irrigation mode property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the area property
			sizeof(u16) +
			// the size of the input property
			sizeof(u16) +
			// the size of the upTime property
			sizeof(u16) +
			// the size of the startTime property
			sizeof(u16);

	return static_cast<CircleMode>(read<u16>(address));
}

inline cu16 metaIrrigation::offsetTime() const {
	// The address offset time property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the area property
			sizeof(u16) +
			// the size of the input property
			sizeof(u16) +
			// the size of the upTime property
			sizeof(u16) +
			// the size of the startTime property
			sizeof(u16) +
			// the size of the irrigation mode property
			sizeof(u16);

	return read<u16>(address);
}

inline cu16 metaIrrigation::dailyPrecipitation() const {
	// The address daily precipitation property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the area property
			sizeof(u16) +
			// the size of the input property
			sizeof(u16) +
			// the size of the upTime property
			sizeof(u16) +
			// the size of the startTime property
			sizeof(u16) +
			// the size of the irrigation mode property
			sizeof(u16) +
			// the size of the offset time property
			sizeof(u16);

	return read<u16>(address);
}

inline cu16 metaIrrigation::unitPrecipitation() const {
	// The address unit precipitation property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the area property
			sizeof(u16) +
			// the size of the input property
			sizeof(u16) +
			// the size of the upTime property
			sizeof(u16) +
			// the size of the startTime property
			sizeof(u16) +
			// the size of the irrigation mode property
			sizeof(u16) +
			// the size of the offset time property
			sizeof(u16) +
			// the size of the daily precipitation property
			sizeof(u16);

	return read<u16>(address);
}

inline cu16 metaIrrigation::repeatCount() const {
	// The address daily repeat count property in the EEPROM is composed of:
	cu32 address =
			// the _itemAddress
			_itemAddress +
			// size of the IO point
			_nameSize +
			// the size of the area property
			sizeof(u16) +
			// the size of the input property
			sizeof(u16) +
			// the size of the upTime property
			sizeof(u16) +
			// the size of the startTime property
			sizeof(u16) +
			// the size of the irrigation mode property
			sizeof(u16) +
			// the size of the offset time property
			sizeof(u16) +
			// the size of the daily precipitation property
			sizeof(u16) +
			// the size of the unit precipitation property
			sizeof(u16);

	return read<u16>(address);
}

} // project
} // manager

#endif // _METAIRRIGATION_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: metairrigation.hpp =====================
