/*
 * devicestatistic.hpp
 *
 *  Created on: 2014.12.31.
 *      Author: moravas
 */

#include "../../projectconfiguration.hpp"

#ifndef _DEVICESTATISTIC_HPP_
#define _DEVICESTATISTIC_HPP_

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

class deviceStatistic {
public: static inline statisticEntry const* current() {return _entries[_index];}
public: static inline bool next() {return _entries[++_index] != 0? true : false;}
public: static inline void reset() {_index = -1;}
private: static s32				_index;
private: static statisticEntry*	_entries[];
};

} // exositeTask
} // namespace manager

#endif // _DEVICESTATISTIC_HPP_
