// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.03.2015.
//! \note
// =============================================================================
#include "metaoutput.hpp"

using namespace manager::project;

void metaOutput::next() {
	// the total item size in the EEPROM composed of:
	_itemAddress +=
			// the size of the address property
			sizeof(u16) +
			// the size of the timeout ON property
			sizeof(u16) +
			// the size of the timeout OFF property
			sizeof(u16) +
			// the size of the input count property
			sizeof(u16) +
			// the number of connected input count property
			inputCount() * sizeof(u16);
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: metainput.cpp =======================
