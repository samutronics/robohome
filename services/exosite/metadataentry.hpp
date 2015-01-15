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

static const u16 metaDataSize	= 256;
static const u8 CIKSize			= 40;
static const u8 serverIPSize	= 6;
static const u8 PAD0Size		= 2;
static const u8 markSize		= 8;
static const u8 UUIDSize		= 12;
static const u8 PAD1Size		= 4;
static const u8 RSVDSize		= 48;
static const u8 MFRSize			= 128;

class metaDataEntry {
public: char cik	[CIKSize];      // our client interface key
public: char server	[serverIPSize];	// ip address of m2.exosite.com (not using DNS at this stage)
public: char pad0	[PAD0Size];     // pad 'server' to 8 bytes
public: char mark	[markSize];     // watermark
public: char uuid	[UUIDSize];     // UUID in ascii
public: char pad1	[PAD1Size];     // pad 'uuid' to 16 bytes
public: char rsvd	[RSVDSize];     // reserved space - pad to ensure mfr is at end of RDK_META_SIZE
public: char mfr	[MFRSize];      // manufacturer data structure
};

} // exositeTask
} // manager

#endif // _METADATAENTRY_H_

// =============================================================================
//! \file
//! \copyright
// ====================== end of file: metadataentry.hpp =======================
