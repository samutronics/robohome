// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#ifndef _METADATA_H_
#define _METADATA_H_

#include "../../projectconfiguration.hpp"

namespace service {
namespace exosite {

enum MetaElements {
	META_CIK,
	META_SERVER,
	META_MARK,
	META_UUID,
	META_MFR,
	META_NONE
};

class metaData {
public: static void defaults();
public: static void init(s32 reset);
public: static void write(u8* write_buffer, u16 srcBytes, u8 element);
public: static void read(u8* read_buffer, u16 destBytes, u8 element);

private: static void readMetaItem(u8* pucBuffer, s32 iLength, s32 iOffset);
private: static void writeMetaItem(u8* pucBuffer, s32 iLength, s32 iOffset);

private: static const char _exomark[];
};

} // exositeTask

} // manager

#endif // _METADATA_H_

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: metadata.hpp =========================
