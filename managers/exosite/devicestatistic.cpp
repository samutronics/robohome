// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			31.12.2014.
//! \note
// =============================================================================
#include "devicestatistic.hpp"

using namespace manager::exositeTask;

statisticEntry sw0		("Button1", "usrsw1", INT, WRITE_ONLY);
statisticEntry sw1		("Button1", "usrsw2", INT, WRITE_ONLY);
statisticEntry tempF	("Temp(F)", "", INT, WRITE_ONLY);
statisticEntry tempC	("Temp(C)", "jtemp", INT, WRITE_ONLY);
statisticEntry ontime	("Time since reset", "ontime", INT, WRITE_ONLY);
statisticEntry led0		("LED D1", "ledd1", INT, READ_WRITE);
statisticEntry led1		("LED D2", "ledd2", INT, READ_WRITE);
statisticEntry location	("Location", "location", STRING, READ_ONLY);
statisticEntry game		(0, "gamestate", HEX, WRITE_ONLY);
statisticEntry email	("Contact Email", "emailaddr", STRING, READ_WRITE);
statisticEntry alert	("Alert Message", "alert", STRING, NONE);

s32				deviceStatistic::_index = -1;

statisticEntry*	deviceStatistic::_entries[] = {
		&sw0,
		&sw1,
		&tempF,
		&tempC,
		&ontime,
		&led0,
		&led1,
		&location,
		&game,
		&email,
		&alert,
		0
};

// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicestatistic.cpp =======================
