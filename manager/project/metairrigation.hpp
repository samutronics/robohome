// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METAIRRIGATION_HPP_
#define _METAIRRIGATION_HPP_

#include "abstractmetasection.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

class metaIrrigation: public abstractMetaSection {
public: inline metaIrrigation(cu16 sectionAddress);
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline metaIrrigation::metaIrrigation(cu16 sectionAddress): abstractMetaSection(sectionAddress) {}

} // project
} // manager

#endif // _METAIRRIGATION_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: metairrigation.hpp =====================
