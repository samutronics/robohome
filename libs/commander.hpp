// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.03.2015.
//! \note
// =============================================================================
#ifndef _COMMANDEXECUTOR_HPP_
#define _COMMANDEXECUTOR_HPP_

#include "commandsiterator.hpp"
#include "../projectconfiguration.hpp"

namespace libs {

class CommandExecutor {
public: static inline bool execute(const CommandsIterator& it);

private: static bool executeRead(const CommandsIterator& it);
private: static bool executeWrite(const CommandsIterator& it);

private: inline CommandExecutor();
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline CommandExecutor::CommandExecutor() {}

inline bool CommandExecutor::execute(const CommandsIterator& it) {
	if(systemGlobal::cmdWrite & it.key()) {
		return executeWrite(it);
	}
	else {
		return executeRead(it);
	}
}

} // libs

#endif // _COMMANDEXECUTOR_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: commandexecutor.hpp ====================
