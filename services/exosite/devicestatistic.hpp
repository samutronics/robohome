// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			31.12.2014.
//! \note
// =============================================================================
#ifndef _DEVICESTATISTIC_HPP_
#define _DEVICESTATISTIC_HPP_

#include "statisticentry.hpp"
#include "../../projectconfiguration.hpp"

namespace service {
namespace exositeTask {

class deviceStatistic {
public: static inline statisticEntry* current() {return _entries[_index];}
public: static inline bool next() {return _entries[++_index] != 0? true : false;}
public: static inline void reset() {_index = -1;}

private: static s32				_index;
private: static statisticEntry*	_entries[];
};

} // exositeTask
} // namespace manager

#endif // _DEVICESTATISTIC_HPP_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: devicestatistic.hpp =======================
