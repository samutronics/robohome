// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			31.12.2014.
//! \note
// =============================================================================
#ifndef _STATISTICENTRY_HPP_
#define _STATISTICENTRY_HPP_

#include "../../projectconfiguration.hpp"

namespace manager {
namespace exositeTask {

enum accessMode {
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE,
    NONE
};

enum valueType {
    STRING,
    INT
};

class statisticEntry {
public: static const u32 dataStringLength = 32;

public: inline statisticEntry(const char* name, const char* alias, valueType t, accessMode a):
		entryName(name), entryAliasInCloud(alias), type(t), access(a) {}

public: const char* entryName;
public: union {
	s32 intValue;
	s8 stringValue[dataStringLength];
} entryValue;

public: const char* entryAliasInCloud;
public: valueType type;
public: accessMode access;
};

} // exositeTask
} // namespace manager

#endif // _STATISTICENTRY_HPP_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: statisticentry.hpp ========================
