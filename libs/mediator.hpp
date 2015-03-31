// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.03.2015.
//! \note
// =============================================================================
#ifndef _COMMANDEXECUTOR_HPP_
#define _COMMANDEXECUTOR_HPP_

#include "irrigation.hpp"
#include "inputmanager.hpp"
#include "outputmanager.hpp"
#include "projectmanager.hpp"
#include "commandsiterator.hpp"
#include "../projectconfiguration.hpp"

namespace libs {

class IInform;

class Mediator {
public: inline bool execute(const CommandsIterator& it, std::string& result) const;
public: inline void attach(cu32 ComponentID, IInform* info);
public: inline void dettach(cu32 ComponentID);

protected: inline Mediator();

private: std::map<u32, IInform*> _components;
};

typedef SingletonFactory<Mediator>	MediatorFactory;

// =============================================================================
// Inline method implementation
// =============================================================================

inline Mediator::Mediator() {
}

inline void Mediator::attach(cu32 ComponentID, IInform* info) {
	_components[ComponentID] = info;
}

inline void Mediator::dettach(cu32 ComponentID) {
	_components.erase(ComponentID);
}

inline bool Mediator::execute(const CommandsIterator& it, std::string& result) const {
	std::map<u32, IInform*>::const_iterator info = _components.find(it.key() & systemGlobal::ComponentIDMask);
	if(!info->second) {
		return false;
	}

	if(systemGlobal::cmdWrite & it.key()) {
		return info->second->write(it);
	}
	else {
		return info->second->read(it, result);
	}
}

} // libs

#endif // _COMMANDEXECUTOR_HPP_
// =============================================================================
//! \file
//! \copyright
// =========================== end of file: commander.hpp ======================
