// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.03.2015.
//! \note
// =============================================================================
#include "metainput.hpp"

using namespace manager::project;

void metaInput::next() {
	_itemAddress += sizeof(u8) + _nameSize;
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: metainput.cpp =======================
