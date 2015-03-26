// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			25.03.2015.
//! \note
// =============================================================================
#include "metairrigation.hpp"

using namespace manager::project;

void metaIrrigation::next() {
	// the total item size in the EEPROM composed of:
	_itemAddress +=
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
}

// =============================================================================
//! \file
//! \copyright
// ======================= end of file: metairrigation.cpp =====================
