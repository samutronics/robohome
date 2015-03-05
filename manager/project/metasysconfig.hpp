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
public: metaSysConfig(cu16 sectionAddress): _sectionAddress(sectionAddress) {}
private: cu16 _sectionAddress;
};

// =============================================================================
// Inline method implementation
// =============================================================================

} // project
} // manager

#endif // _METASYSCONFIG_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: metasysconfig.hpp =====================
