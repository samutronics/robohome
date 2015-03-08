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

// =============================================================================
//! \file
//! \copyright
// ======================= end of file: projectmanager.cpp =====================
