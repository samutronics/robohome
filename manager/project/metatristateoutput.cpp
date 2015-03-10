// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.03.2015.
//! \note
// =============================================================================
#include "metatristateoutput.hpp"

using namespace manager::project;

void metaTriStateOutput::next() {
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

// =============================================================================
//! \file
//! \copyright
// =================== end of file: metatristateoutput.cpp =====================
