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

enum InputType {
	BothEdges,
	RisingEdge,
	DeferredBothEdges,
	DeferredRisingEdge
};

class metaInput: public abstractMetaIO {
public: inline metaInput(const u16 sectionAddress);

public: virtual void next();
public: inline InputType trigger() const;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline metaInput::metaInput(cu16 sectionAddress): abstractMetaIO(sectionAddress) {}

inline InputType metaInput::trigger() const {
	return static_cast<InputType>(read<u8>(_itemAddress + _nameSize));
}

} // project
} // manager

#endif // _METAINPUT_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: metainput.hpp =======================
