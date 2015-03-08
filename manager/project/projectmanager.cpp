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
		UARTprintf("%d %d %d %d\n", ((project[index] >> 24) & 0XFF), ((project[index] >> 16) & 0XFF), ((project[index] >> 8) & 0XFF), (project[index] & 0XFF));
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
	string str;
	sys.cik(str);
	UARTprintf("CIK: %s\n", str.c_str());

	metaInput in = input();
	UARTprintf("Total number of inputs: %d\n", in.totalCount());
	for(u32 index = 0; index < in.totalCount(); index++) {
		UARTprintf("\tType of input at address %d: %s\n",
				index,
				in.trigger() == triggerBothEdges ? "triggerBothEdges" :
						in.trigger() == triggerRisingEdge ? "triggerRisingEdge" : "none");
		in.next();
	}

	metaOutput out = output();
	UARTprintf("Total number of simple outputs: %d\n", out.totalCount());
	for(u32 index = 0; index < out.totalCount(); index++) {
		UARTprintf("\tAddress of output: %d\n", out.address());
		UARTprintf("\tTime-out of output: %d\n", out.timeout());
		vector<u16> inputs;
		out.inputs(inputs);
		UARTprintf("\tAccessed from inputs: ");
		for(u32 item = 0; item < inputs.size(); item++) {
			UARTprintf("%d ", inputs[item]);
		}

		UARTprintf("\n");
		out.next();
	}

}

// =============================================================================
//! \file
//! \copyright
// ======================= end of file: projectmanager.cpp =====================
