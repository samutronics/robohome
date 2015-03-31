// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#include "mediator.hpp"
#include "inputmanager.hpp"

using namespace libs;
using namespace systemGlobal;
using namespace manager::input;
using namespace service::inbound::configuration;

InputManager::InputManager():
				_inputs(project::ProjectManagerFactory::get()->input().count()),
				_dataChanged((_inputs.size() / (sizeof(_dataChanged[0]) * 8)) + ((_inputs.size() % (sizeof(_dataChanged[0]) * 8)) ? 1 : 0), 0),
				_dataCurrent(_dataChanged.size(), 0),
				_dataPrevious(_dataChanged.size(), 0),
				_lock(_dataChanged.size()) {
	project::metaInput data = project::ProjectManagerFactory::get()->input();
	for(u32 index = 0; index < data.count(); index++) {
		_inputs[index] = new Input(data.trigger(), index, _dataChanged, _dataCurrent, _dataPrevious);
		data.next();
	}

	for(u32 index = 0; index < _dataChanged.size(); index++) {_lock[index] = xSemaphoreCreateMutex();}

	MediatorFactory::get()->attach(ComponentIDInput, this);
}

InputManager::~InputManager() {
	for(u32 index = 0; index < _inputs.size(); index++) {delete _inputs[index];}

	for(u32 index = 0; index < _lock.size(); index++) {vSemaphoreDelete(_lock[index]);}
}

bool InputManager::write(const CommandsIterator& it) {
	write(it.key() & addressMask, strtoul(it.value().c_str(), NULL, 10));
	return true;
}

bool InputManager::read(const CommandsIterator& it, std::string& result) const {
	return false;
}

// =============================================================================
//! \file
//! \copyright
// ======================== end of file: inputmanager.cpp ======================
