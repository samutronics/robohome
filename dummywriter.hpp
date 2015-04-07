// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.03.2015.
//! \note
// =============================================================================
#ifndef DUMMYWRITER_HPP_
#define DUMMYWRITER_HPP_

#include "projectconfiguration.hpp"

//make system configuration
static cs8 cik[] = "15e4890a88d79261608b194c8217c486c68f0007";
static cu16 cikSize = (sizeof(cik) / sizeof(cik[0]));

inline void makeSysCfg(std::vector<u8>& project) {
	project.push_back(16);	// write hw input count
	project.push_back(0);	// write hw input count
	project.push_back(16);	// write hw output count
	project.push_back(0);	// write hw output count

	project.push_back(10);	// write IP address
	project.push_back(1);	// write IP address
	project.push_back(168);	// write IP address
	project.push_back(192);	// write IP address

	project.push_back(0);	// write netmask
	project.push_back(255);	// write netmask
	project.push_back(255);	// write netmask
	project.push_back(255);	// write netmask

	project.push_back(1);	// write default gateway
	project.push_back(1);	// write default gateway
	project.push_back(168);	// write default gateway
	project.push_back(192);	// write default gateway

	project.push_back(static_cast<u8>(cikSize & 0XFF));
	project.push_back(static_cast<u8>((cikSize >> 8) & 0XFF));
	for(u32 index = 0; index < sizeof(cik); index++) {
		project.push_back(cik[index]);
	}
}

//make input properties
static cu8 input[] = {
		manager::project::RisingEdge,
		manager::project::RisingEdge,
		manager::project::BothEdges,
		manager::project::BothEdges,
		manager::project::RisingEdge,
		manager::project::RisingEdge,
		manager::project::RisingEdge,
		manager::project::RisingEdge,
		manager::project::DeferredBothEdges,
		manager::project::DeferredBothEdges,
		manager::project::DeferredBothEdges,
		manager::project::DeferredBothEdges,
		manager::project::DeferredRisingEdge,
		manager::project::DeferredRisingEdge,
		manager::project::DeferredRisingEdge,
		manager::project::DeferredRisingEdge,
		manager::project::BothEdges,
		manager::project::BothEdges,
		manager::project::BothEdges
};

static const u8* inputname[] = {
		"RisingEdge      ",
		"RisingEdge      ",
		"BothEdges       ",
		"BothEdges       ",
		"RisingEdge      ",
		"RisingEdge      ",
		"RisingEdge      ",
		"RisingEdge      ",
		"DeferredBothEdge",
		"DeferredBothEdge",
		"DeferredBothEdge",
		"DeferredBothEdge",
		"DeferredRisingEd",
		"DeferredRisingEd",
		"DeferredRisingEd",
		"DeferredRisingEd",
		"BothEdges       ",
		"BothEdges       ",
		"BothEdges       "
};

inline void makeInputCfg(std::vector<u8>& project) {
	cu16 inputCount = sizeof(input) / sizeof(input[0]);
	project.push_back(static_cast<u8>(inputCount & 0XFF));
	project.push_back(static_cast<u8>((inputCount >> 8) & 0XFF));
	for(u32 index = 0; index < sizeof(input); index++) {
		for(u32 c = 0; c < 16; c++) {
			project.push_back(inputname[index][c]);
		}

		project.push_back(input[index]);
	}
}

//make simple output properties
static cu16 simpleO[] = {
		//name						address        timeOn        TimeOff        inputCount        inputList
		0, 0, 0, 0, 0, 0, 0, 0,		0,                0,          0,     	       	1,                16,
		0, 0, 0, 0, 0, 0, 0, 0,		1,                0,          0,        	    1,                17,
		0, 0, 0, 0, 0, 0, 0, 0,		2,                0,          0,	      		1,                18,
		0, 0, 0, 0, 0, 0, 0, 0,		3,                0,          100,        		1,                5,
		0, 0, 0, 0, 0, 0, 0, 0,		4,                100,        0,            	1,                8,
		0, 0, 0, 0, 0, 0, 0, 0,		5,                100,        0,            	1,                12,
		0, 0, 0, 0, 0, 0, 0, 0,		6,                100,        100,        		1,                9,
		0, 0, 0, 0, 0, 0, 0, 0,		7,                100,        100,        		1,                13,
		0, 0, 0, 0, 0, 0, 0, 0,		8,                100,        0,            	2,                2, 10,
		0, 0, 0, 0, 0, 0, 0, 0,		9,                100,        0,            	2,                6, 11,
		0, 0, 0, 0, 0, 0, 0, 0,		10,               100,        100,        		2,                3, 14,
		0, 0, 0, 0, 0, 0, 0, 0,		11,               100,        100,        		2,                7, 15
};

static cu16 simpleOutputCount = 3;

inline void makeSimpleOutputCfg(std::vector<u8>& project) {
	project.push_back(static_cast<u8>(simpleOutputCount & 0XFF));
	project.push_back(static_cast<u8>((simpleOutputCount >> 8) & 0XFF));
	for(u32 index = 0; index < sizeof(simpleO) / sizeof(simpleO[0]); index++) {
		project.push_back(static_cast<u8>(simpleO[index] & 0XFF));
		project.push_back(static_cast<u8>((simpleO[index] >> 8) & 0XFF));
	}
}

//make tri-state output properties
static cu16 triStateO[] = {
		// name						address, timeOn, timeOff,	UpDown-count, UpDown list,	extendedaddress, Down-count, Down list, Up-count, Up list,
		0, 0, 0, 0, 0, 0, 0, 0,		1,			0,		0,			1,				0,			2,				0,						0,
		0, 0, 0, 0, 0, 0, 0, 0,		3,			0,		100,		1,				1,			4,				2,			7, 15,		2,		6, 14,
		0, 0, 0, 0, 0, 0, 0, 0,		4,			100,	100,		1,				2,			5,				0,						0
};

static cu16 triStateOutputCount = 0;

inline void makeTriStateOutputCfg(std::vector<u8>& project) {
	project.push_back(static_cast<u8>(triStateOutputCount & 0XFF));
	project.push_back(static_cast<u8>((triStateOutputCount >> 8) & 0XFF));
	for(u32 index = 0; index < sizeof(triStateO) / sizeof(triStateO[0]); index++) {
		project.push_back(static_cast<u8>(triStateO[index] & 0XFF));
		project.push_back(static_cast<u8>((triStateO[index] >> 8) & 0XFF));
	}
}

static cu16 irrigation[] = {
		// name						area, input, up time,	start time, 		mode,				offset time, dailyPrecipitation,	unitPrecipitation,	repeat
		0, 0, 0, 0, 0, 0, 0, 0,		10,		16,		10,			1,		manager::project::Normal,		20,			10,						1,				1,
		0, 0, 0, 0, 0, 0, 0, 0,		10,		18,		10,		42300,		manager::project::Grown,		20,			1500,					10,				3,
		0, 0, 0, 0, 0, 0, 0, 0,		10,		17,		10,			1,		manager::project::Normal,		20,			2000,					10,				1
};

static cu16 irrigationCount = 3;

inline void makeIrrigationCfg(std::vector<u8>& project) {
	project.push_back(static_cast<u8>(irrigationCount & 0XFF));
	project.push_back(static_cast<u8>((irrigationCount >> 8) & 0XFF));
	for(u32 index = 0; index < sizeof(irrigation) / sizeof(irrigation[0]); index++) {
		project.push_back(static_cast<u8>(irrigation[index] & 0XFF));
		project.push_back(static_cast<u8>((irrigation[index] >> 8) & 0XFF));
	}
}

inline void makeCfg(std::vector<u32>& project) {
	std::vector<u8> data;
	cu16 sysCfgAddress		= 8; // start directly after the main section table
	makeSysCfg(data);
	cu16 inputAddress		= sysCfgAddress + data.size();
	makeInputCfg(data);
	cu16 outputAddress		= sysCfgAddress + data.size();

	// push the total size of sections
	data.push_back(static_cast<u8>((sizeof(simpleO) + sizeof(simpleOutputCount)) & 0XFF));
	data.push_back(static_cast<u8>(((sizeof(simpleO) + sizeof(simpleOutputCount))>> 8) & 0XFF));
	data.push_back(static_cast<u8>((sizeof(triStateO) + sizeof(triStateOutputCount)) & 0XFF));
	data.push_back(static_cast<u8>(((sizeof(triStateO) + sizeof(triStateOutputCount)) >> 8) & 0XFF));

	makeSimpleOutputCfg(data);
	makeTriStateOutputCfg(data);
	cu16 irrigationAddress	= sysCfgAddress + data.size();

	makeIrrigationCfg(data);

	project.push_back((inputAddress << 16) | sysCfgAddress);
	project.push_back((irrigationAddress << 16) | outputAddress);
	for(u32 index = 0; index < data.size() - (data.size() % sizeof(project[0])); index += sizeof(project[0])) {
		project.push_back(data[index + 3] << 24 | data[index + 2] << 16 | data[index + 1] << 8 | data[index]);
	}

	if(data.size() % sizeof(project[0])) {
		u32 last = 0;
		for(u32 index = data.size() - (data.size() % sizeof(project[0])); index < data.size(); index++) {
			last |= data[index] << ((index % sizeof(project[0])) * 8);
		}

		project.push_back(last);
	}
}

#endif // DUMMYWRITER_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: dummywriter.hpp ======================
