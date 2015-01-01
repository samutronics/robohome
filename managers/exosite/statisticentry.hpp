// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			31.12.2014.
//! \note
// =============================================================================
#ifndef _STATISTICENTRY_HPP_
#define _STATISTICENTRY_HPP_

#include "ustdlib.h"
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
    INT,
    HEX
};

class statisticEntry {
public: static const u32 dataStringLength = 32;

public: inline statisticEntry(const char* name, const char* alias, valueType t, accessMode a):
		entryName(name), entryAliasInCloud(alias), type(t), access(a) {}

public: inline void requestFormat(char* requestBuffer) const;

public: inline void setValue(char* requestBuffer);
public: inline bool getValue(char* requestBuffer) const;
public: inline bool getValue(s32& requestBuffer) const;
private: const char* entryName;
private: s8 value[dataStringLength];
private: const char* entryAliasInCloud;
private: valueType type;
private: accessMode access;
};

// =============================================================================
// section for impelentation of inpline methods
// =============================================================================

inline void statisticEntry::setValue(char* requestBuffer) {
	strncpy(value, requestBuffer, dataStringLength);
}

inline bool statisticEntry::getValue(char* requestBuffer) const {
	if(STRING != type) {return false;}
	strncpy(requestBuffer, value, dataStringLength);
	return true;
}

inline bool statisticEntry::getValue(s32& requestBuffer) const {
	switch (type) {
	case STRING: {
		return false;
	}
	case INT: {
		requestBuffer = strtol(value, 0, 10);
		break;
	}
	case HEX: {
		requestBuffer = strtol(value, 0, 16);
		break;
	}
	default: {
		while(true);
	}
	}

	return true;
}

inline void statisticEntry::requestFormat(char* requestBuffer) const {
	switch (type) {
	case STRING: {
#pragma warning "synchronization may require";
		usprintf(requestBuffer, "%s=%s", entryAliasInCloud, value);
		break;
	}
	case INT: {
#pragma warning "check the correct alignement in the value string"
		usprintf(requestBuffer, "%s=%d", entryAliasInCloud, strtol(value, 0, 10));
		break;
	}
	case HEX: {
#pragma warning "check the correct alignement in the value string"
		usprintf(requestBuffer, "%s=0x%x", entryAliasInCloud, strtol(value, 0, 16));
		break;
	}
	default: {
		while(true);
	}
}

} // exositeTask
} // namespace manager

#endif // _STATISTICENTRY_HPP_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: statisticentry.hpp ========================
