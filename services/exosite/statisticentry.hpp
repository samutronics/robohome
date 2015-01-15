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
namespace exosite {

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
public: inline statisticEntry(const char* name, const char* alias, valueType t, accessMode a):
				entryName(name), entryAliasInCloud(alias), type(t), access(a) {
	_value.clear();
	_value.reserve(32);
}

public: inline void requestFormat(std::string& str) const;

public: inline void setValue(const std::string& value);
public: inline const std::string& getValue() const;
//public: inline s32 getValue() const;

public: const char* entryName;
public: const char* entryAliasInCloud;
public: accessMode access;

private: std::string _value;
private: valueType type;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline void statisticEntry::setValue(const std::string& value) {
	_value = value;
}

inline const std::string& statisticEntry::getValue() const {
	return _value;
}
/*
inline s32 statisticEntry::getValue() const {
	switch (type) {
	case STRING: {
		return 0;
	}
	case INT: {
		return std::stoi(_value, 0, 10);
	}
	case HEX: {
		return std::stoi(_value, 0, 16);
	}
	default: {
		while(true);
	}
	}
}
*/
inline void statisticEntry::requestFormat(std::string& str) const {
	str.append(entryAliasInCloud);
	str += '=';
	switch (type) {
	case STRING: {
		str.append(_value);
		break;
	}
	case INT: {
		str.append(_value);
//		sprintf(requestBuffer, "%s=%d", entryAliasInCloud, strtol(_value, 0, 10));
		break;
	}
	case HEX: {
		str.append(_value);
//		sprintf(requestBuffer, "%s=0x%x", entryAliasInCloud, strtol(value, 0, 16));
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
