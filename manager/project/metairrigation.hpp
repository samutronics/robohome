// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METAIRRIGATION_HPP_
#define _METAIRRIGATION_HPP_

#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class metaIrrigation {
public: metaIrrigation(cu16 sectionAddress): _sectionAddress(sectionAddress) {}
protected: cu16 _sectionAddress;
};

// =============================================================================
// Inline method implementation
// =============================================================================

} // project
} // manager

#endif // _METAIRRIGATION_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: metairrigation.hpp =====================
