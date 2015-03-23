// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#include "outputmanager.hpp"

using namespace manager::output;

OutputManager* OutputManager::_instance = NULL;

OutputManager::~OutputManager() {
	for(std::map<u32, Output*>::iterator it = _output.begin(); it != _output.end(); ++it) {delete it->second;}
}
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: outputmanager.cpp ======================
