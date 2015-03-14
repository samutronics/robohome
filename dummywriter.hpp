// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.03.2015.
//! \note
// =============================================================================
#ifndef DUMMYWRITER_HPP_
#define DUMMYWRITER_HPP_

#include "../projectconfiguration.hpp"

//make system configuration
static cs8 cik[] = "15e4890a88d79261608b194c8217c486c68f0007";
static cu16 cikSize = (sizeof(cik) / sizeof(cik[0]));

inline void makeSysCfg(std::vector<u8>& project) {
	project.push_back(16);	// write hw input count
	project.push_back(0);	// write hw input count
	project.push_back(16);	// write hw output count
	project.push_back(0);	// write hw output count

	project.push_back(11);	// write IP address
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
		manager::project::triggerRisingEdge,
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

inline void makeInputCfg(std::vector<u8>& project) {
	cu16 inputCount = sizeof(input) / sizeof(input[0]);
	project.push_back(static_cast<u8>(inputCount & 0XFF));
	project.push_back(static_cast<u8>((inputCount >> 8) & 0XFF));
	for(u32 index = 0; index < sizeof(input); index++) {
		project.push_back(input[index]);
	}
}

//make simple output properties
static cu16 simpleO[] = {
		// address, time-out, input-count, input list
		0,	0,		1,		0,
		1,	200,	2,		1, 2,
		2,	0,		2,		4, 6,
		3,	0,		1,		7
};

static cu16 simpleOutputCount = 4;

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
		// address, time-out, UpDown-count, UpDown list, extendedaddress, Down-count, Down list, Up-count, Up list,
		4,	500,	1,		5,		5,		0,				0,
		6,	0,		0,				7,		1,		8,		1,		9,
		8,	0,		0,				9,		2,		10, 11,	2,		12, 13
};

static cu16 triStateOutputCount = 3;

inline void makeTriStateOutputCfg(std::vector<u8>& project) {
	project.push_back(static_cast<u8>(triStateOutputCount & 0XFF));
	project.push_back(static_cast<u8>((triStateOutputCount >> 8) & 0XFF));
	for(u32 index = 0; index < sizeof(triStateO) / sizeof(triStateO[0]); index++) {
		project.push_back(static_cast<u8>(triStateO[index] & 0XFF));
		project.push_back(static_cast<u8>((triStateO[index] >> 8) & 0XFF));
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
