// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.03.2015.
//! \note
// =============================================================================
#ifndef DUMMYWRITER_HPP_
#define DUMMYWRITER_HPP_

#include "metainput.hpp"
#include "../projectconfiguration.hpp"

//make system configuration
static cs8 cik[] = "15e4890a88d79261608b194c8217c486c68f0007";
static cu16 cikSize = (sizeof(cik) / sizeof(cik[0]));

//make input properties
static cu8 input[] = {
		manager::project::triggerBothEdges,
		manager::project::triggerBothEdges,
		manager::project::triggerBothEdges,
		manager::project::triggerBothEdges,
		manager::project::triggerBothEdges,
		manager::project::triggerBothEdges,
		manager::project::triggerBothEdges,
		manager::project::triggerBothEdges,
		manager::project::triggerRisingEdge,
		manager::project::triggerRisingEdge,
		manager::project::triggerRisingEdge,
		manager::project::triggerRisingEdge,
		manager::project::triggerRisingEdge,
		manager::project::triggerRisingEdge,
		manager::project::triggerRisingEdge,
		manager::project::triggerRisingEdge
};

static cu16 inputCount = sizeof(input) / sizeof(input[0]);

//make simple output properties
static cu16 simpleO[] = {
		// address, time-out, input-count, input list
		0,	0,		1,		0,
		1,	200,	2,		1, 2,
		2,	0,		3,		4, 6,
		3,	0,		1,		7
};

static cu16 simpleOutputCount = sizeof(simpleO) / sizeof(simpleO[0]);

//make tri-state output properties
static cu16 triStateO[] = {
		// address, time-out, UpDown-count, UpDown list, extendedaddress, Down-count, Down list, Up-count, Up list,
		4,	500,	1,		5,		5,		0,		0,		0,		0,
		6,	0,		0,				7,		1,		8,		1,		9,
		8,	0,		0,				9,		2,		10, 11,	2,		12, 13
};

static cu16 triStateOutputCount = sizeof(triStateO) / sizeof(triStateO[0]);

static cu16 sysCfgAddress		= 8; // start directly after the sections
static cu16 inputAddress		= sizeof(cikSize) + sizeof(cik);
static cu16 outputAddress		= inputAddress + sizeof(inputCount) + sizeof(input);
static cu16 irrigationAddress	= outputAddress + sizeof(simpleOutputCount) + sizeof(triStateOutputCount) + sizeof(simpleO) + sizeof(triStateO);

inline void writeDummyProject() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	EEPROMInit();

}

#endif // DUMMYWRITER_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: dummywriter.hpp ======================
