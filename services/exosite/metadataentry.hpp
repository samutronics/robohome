// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#ifndef _METADATAENTRY_H_
#define _METADATAENTRY_H_

#include "../../projectconfiguration.hpp"

namespace service {
namespace exosite {

static cu16 metaDataSize	= 256;
static cu8 CIKSize			= 40;
static cu8 serverIPSize		= 6;
static cu8 PAD0Size			= 2;
static cu8 markSize			= 8;
static cu8 UUIDSize			= 12;
static cu8 PAD1Size			= 4;
static cu8 RSVDSize			= 48;
static cu8 MFRSize			= 128;

class metaDataEntry {
public: s8 cik		[CIKSize];      // our client interface key
public: s8 server	[serverIPSize];	// ip address of m2.exosite.com (not using DNS at this stage)
public: s8 pad0		[PAD0Size];     // pad 'server' to 8 bytes
public: s8 mark		[markSize];     // watermark
public: s8 uuid		[UUIDSize];     // UUID in ascii
public: s8 pad1		[PAD1Size];     // pad 'uuid' to 16 bytes
public: s8 rsvd		[RSVDSize];     // reserved space - pad to ensure mfr is at end of RDK_META_SIZE
public: s8 mfr		[MFRSize];      // manufacturer data structure
};

} // exositeTask
} // manager

#endif // _METADATAENTRY_H_

// =============================================================================
//! \file
//! \copyright
// ====================== end of file: metadataentry.hpp =======================
