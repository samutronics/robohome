// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#include "projectmanager.hpp"

using namespace std;
using namespace manager::project;

ProjectManager* ProjectManager::_instance;

void ProjectManager::read(vector<u32>& project) const {
	project.reserve(EEPROMSizeGet() / sizeof(project[0]));
	for(u32 index = 0; index < EEPROMSizeGet() / sizeof(project[0]); index++) {
		u32 tmp;
		EEPROMRead(&tmp, index * sizeof(tmp), sizeof(tmp));
		project.push_back(tmp);
	}
}

bool ProjectManager::write(vector<u32>& project) const {
	if((EEPROMSizeGet() / sizeof(project[0])) < project.size()) {return false;}
	for(u32 index = 0; index < project.size(); index++) {
		EEPROMProgram(&project[index], index * sizeof(project[0]), sizeof(project[0]));
	}
	return true;
}

void ProjectManager::trace() const {
	vector<u32> project;
	read(project);

	for(u32 index = 0; index < ((project.size() / 2) * 2); index +=2) {
		UARTprintf("%d\t%d\t%d\t%d\t|\t%d\t%d\t%d\t%d\t|\t%c%c%c%c %c%c%c%c\n",
				((project[index] >> 24) & 0XFF),
				((project[index] >> 16) & 0XFF),
				((project[index] >> 8) & 0XFF),
				((project[index]) & 0XFF),
				((project[index+1] >> 24) & 0XFF),
				((project[index+1] >> 16) & 0XFF),
				((project[index+1] >> 8) & 0XFF),
				((project[index+1]) & 0XFF),
				((project[index] >> 24) & 0XFF),
				((project[index] >> 16) & 0XFF),
				((project[index] >> 8) & 0XFF),
				((project[index]) & 0XFF),
				((project[index+1] >> 24) & 0XFF),
				((project[index+1] >> 16) & 0XFF),
				((project[index+1] >> 8) & 0XFF),
				((project[index+1]) & 0XFF));
	}

	if(project.size() % 2) {
		UARTprintf("%d\t%d\t%d\t%d\t|\t%d\t%d\t%d\t%d\t|\t%c%c%c%c %c%c%c%c\n",
				((project[project.size() - 1] >> 24) & 0XFF),
				((project[project.size() - 1] >> 16) & 0XFF),
				((project[project.size() - 1] >> 8) & 0XFF),
				((project[project.size() - 1]) & 0XFF),
				(0),
				(0),
				(0),
				(0),
				((project[project.size() - 1] >> 24) & 0XFF),
				((project[project.size() - 1] >> 16) & 0XFF),
				((project[project.size() - 1] >> 8) & 0XFF),
				((project[project.size() - 1]) & 0XFF),
				(0),
				(0),
				(0),
				(0));
	}
}

void ProjectManager::parse() const {
	vector<u32> project;
	read(project);

	UARTprintf("Section table:\n\tSystem configuration: %d\n\tInput properties: %d\n\tOutput properties: %d\n\tIrrigation configuration: %d\n",
			static_cast<u16>(project[0] & 0XFFFF),
			static_cast<u16>((project[0] >> 16) & 0XFFFF),
			static_cast<u16>(project[1] & 0XFFFF),
			static_cast<u16>((project[1] >> 16) & 0XFFFF));

	metaSysConfig sys = sysConfig();
	UARTprintf("Hardware input count: %d\n", sys.hwInputNumber());
	UARTprintf("Hardware output count: %d\n", sys.hwOutputNumber());
	UARTprintf("Network configuration:\n");

	u32 ip;
	u32 netmask;
	u32 gateway;
	sys.network(ip, netmask, gateway);
	UARTprintf("\tIP address:\t%d.%d.%d.%d\n", ((ip >> 24) & 0XFF), ((ip >> 16) & 0XFF), ((ip >> 8) & 0XFF), (ip & 0XFF));
	UARTprintf("\tNetwork mask:\t%d.%d.%d.%d\n", ((netmask >> 24) & 0XFF), ((netmask >> 16) & 0XFF), ((netmask >> 8) & 0XFF), (netmask & 0XFF));
	UARTprintf("\tDefault gatewy:\t%d.%d.%d.%d\n", ((gateway >> 24) & 0XFF), ((gateway >> 16) & 0XFF), ((gateway >> 8) & 0XFF), (gateway & 0XFF));

	string str;
	sys.cik(str);
	UARTprintf("CIK: %s\n", str.c_str());

	metaInput in = input();
	UARTprintf("Total number of inputs: %d\n", in.count());
	for(u32 index = 0; index < in.count(); index++) {
		string str;
		in.name(str);
		UARTprintf("\tInput %d: %s, %s\n",
				index,
				str.c_str(),
				in.trigger() == BothEdges ? "BothEdges" :
						in.trigger() == RisingEdge ? "RisingEdge" :
								in.trigger() == DeferredBothEdges ? "DeferredBothEdges" :
										in.trigger() == DeferredRisingEdge ? "DeferredRisingEdge" : "triggerUnknown");
		in.next();
	}

	metaOutput out = output();
	UARTprintf("Total number of simple outputs: %d\n", out.count());
	for(u32 index = 0; index < out.count(); index++) {
		string str;
		out.name(str);
		UARTprintf("\tAddress of %s: %d\n", str.c_str(), out.address());
		UARTprintf("\tON/OFF time of output: %d/%d\n", out.timeoutON(), out.timeoutOFF());
		vector<u16> inputs;
		out.inputs(inputs);
		UARTprintf("\tAccessed from inputs: ");
		for(u32 item = 0; item < inputs.size(); item++) {
			UARTprintf("%d ", inputs[item]);
		}

		UARTprintf("\n\n");
		out.next();
	}

	metaTriStateOutput tristate = triStateOutput();
	UARTprintf("Total number of tri-state outputs: %d\n", tristate.count());
	for(u32 index = 0; index < tristate.count(); index++) {
		string str;
		tristate.name(str);
		UARTprintf("\tAddress of %s: %d\n", str.c_str(), tristate.address());
		UARTprintf("\tExtended address of ti-state output: %d\n", tristate.extendedAddress());
		UARTprintf("\tON/OFF time of output: %d/%d\n", tristate.timeoutON(), tristate.timeoutOFF());
		vector<u16> inputs;
		tristate.inputs(inputs);
		UARTprintf("\tAccessed from UPDown inputs: ");
		for(u32 item = 0; item < inputs.size(); item++) {
			UARTprintf("%d ", inputs[item]);
		}

		UARTprintf("\n");

		inputs.clear();
		tristate.inputsUp(inputs);
		UARTprintf("\tAccessed from UP inputs: ");
		for(u32 item = 0; item < inputs.size(); item++) {
			UARTprintf("%d ", inputs[item]);
		}

		UARTprintf("\n");

		inputs.clear();
		tristate.inputsDown(inputs);
		UARTprintf("\tAccessed from Down inputs: ");
		for(u32 item = 0; item < inputs.size(); item++) {
			UARTprintf("%d ", inputs[item]);
		}

		UARTprintf("\n\n");
		tristate.next();
	}

	metaIrrigation irr = irrigation();
	UARTprintf("Total round of irrigations: %d\n", irr.count());
	for(u32 index = 0; index < irr.count(); index++) {
		string str;
		irr.name(str);
		UARTprintf("\tIrrigated area name: %s\n", str.c_str());
		UARTprintf("\tIrrigated area size: %d\n", irr.area());
		UARTprintf("\tMode: %s\n", Normal == irr.mode() ? "Normal" : "Grown");
		UARTprintf("\tAssigned to input: %d\n", irr.input());
		UARTprintf("\tDaily start: %d\n", irr.startTime());
		UARTprintf("\tUptime: %d\n", irr.upTime());
		UARTprintf("\tOffset between uptimes: %d\n", irr.offsetTime());
		UARTprintf("\tUnit and total precipitation: %d/%d\n\n", irr.unitPrecipitation(), irr.dailyPrecipitation());

		irr.next();
	}
}

// =============================================================================
//! \file
//! \copyright
// ======================= end of file: projectmanager.cpp =====================
