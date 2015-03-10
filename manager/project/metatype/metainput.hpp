// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			28.02.2015.
//! \note
// =============================================================================
#ifndef _METAINPUT_HPP_
#define _METAINPUT_HPP_

#include "abstractmetaio.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace project {

enum TriggerType {
	triggerBothEdges,
	triggerRisingEdge,
	triggerUnknown
};

class metaInput: public abstractMetaIO {
public: inline metaInput(const u16 sectionAddress);

public: virtual void next();
public: inline TriggerType trigger() const;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline metaInput::metaInput(cu16 sectionAddress): abstractMetaIO(sectionAddress) {}

inline TriggerType metaInput::trigger() const {
	return static_cast<TriggerType>(read<u8>(_itemAddress));
}

} // project
} // manager

#endif // _METAINPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: metainput.hpp =======================
