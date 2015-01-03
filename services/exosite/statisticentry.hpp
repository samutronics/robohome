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

namespace service {
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
				entryName(name), entryAliasInCloud(alias), type(t), access(a) {
	memset(value, 0, dataStringLength);
}

public: inline void requestFormat(char* requestBuffer) const;

public: inline void setValue(const char* requestBuffer);
public: inline bool getValue(char* requestBuffer) const;
public: inline bool getValue(s32& requestBuffer) const;

public: const char* entryName;
public: const char* entryAliasInCloud;
public: accessMode access;

private: s8 value[dataStringLength];
private: valueType type;
};

// =============================================================================
// section for impelentation of inpline methods
// =============================================================================

inline void statisticEntry::setValue(const char* requestBuffer) {
	strcpy(value, requestBuffer);
}

inline bool statisticEntry::getValue(char* requestBuffer) const {
	strcpy(requestBuffer, value);
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
		sprintf(requestBuffer, "%s=%s", entryAliasInCloud, value);
		break;
	}
	case INT: {
		sprintf(requestBuffer, "%s=%d", entryAliasInCloud, strtol(value, 0, 10));
		break;
	}
	case HEX: {
		sprintf(requestBuffer, "%s=0x%x", entryAliasInCloud, strtol(value, 0, 16));
		break;
	}
	default: {
		while(true);
	}
}
}

} // exositeTask
} // namespace manager

#endif // _STATISTICENTRY_HPP_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: statisticentry.hpp ========================
