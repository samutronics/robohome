// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.03.2015.
//! \note
// =============================================================================
#ifndef _IINFORM_HPP_
#define _IINFORM_HPP_

#include "../projectconfiguration.hpp"

namespace libs {

class CommandsIterator;

class IInform {
public: virtual bool write(const CommandsIterator& it) = 0;
public: virtual bool read(const CommandsIterator& it, std::string& result) const = 0;
};

} // libs

#endif // _IINFORM_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: iinform.hpp ==========================
