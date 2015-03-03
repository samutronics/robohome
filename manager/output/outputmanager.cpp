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
	for(u32 index = 0; index < _output.size(); index++) {delete _output[index];}
	for(u32 index = 0; index < _tristateoutput.size(); index++) {delete _tristateoutput[index];}
}
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: outputmanager.cpp ======================