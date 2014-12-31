/*
 * devicestatistic.cpp
 *
 *  Created on: 2014.12.31.
 *      Author: moravas
 */

#include <devicestatistic.hpp>

using namespace manager::exositeTask;

statisticEntry led0("LED D1", "ledd1", INT, READ_WRITE);
statisticEntry led1("LED D2", "ledd2", INT, READ_WRITE);

s32				deviceStatistic::_index = -1;
statisticEntry*	deviceStatistic::_entries[] = {
		&led0,
		&led1,
		0
};
