// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			31.12.2014.
//! \note
// =============================================================================
#include "devicestatistic.hpp"

using namespace manager::exositeTask;

statisticEntry led0("LED D1", "ledd1", INT, READ_WRITE);
statisticEntry led1("LED D2", "ledd2", INT, READ_WRITE);

s32				deviceStatistic::_index = -1;
statisticEntry*	deviceStatistic::_entries[] = {
		&led0,
		&led1,
		0
};

// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicestatistic.cpp =======================
