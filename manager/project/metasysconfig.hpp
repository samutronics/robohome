// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METASYSCONFIG_HPP_
#define _METASYSCONFIG_HPP_

#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class metaSysConfig {
public: metaSysConfig(const u16 sectionAddress): _sectionAddress(sectionAddress) {}
private: const u16 _sectionAddress;
};

} // project
} // manager

#endif // _METASYSCONFIG_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: metasysconfig.hpp =====================
