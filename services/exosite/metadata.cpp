// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#include "metadata.hpp"
#include "metadataentry.hpp"

using namespace service::exositeTask;

const char metaData::_exomark[] = "exosite!";

void metaData::defaults() {
	const unsigned char meta_server_ip[6] = {173,255,209,28,0,80};

	write((unsigned char *)meta_server_ip, 6, META_SERVER);     //store server IP
	write((unsigned char *)_exomark, sizeof(_exomark) - 1, META_MARK); //store exosite mark
	return;
}

void metaData::init(s32 reset) {
	char strBuf[sizeof(_exomark) - 1];

	//
	// Enable the EEPROM peripheral.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);

	//
	// Initialize the EEPROM
	//
	EEPROMInit();

	//check our meta mark - if it isn't there, we wipe the meta structure
	read((unsigned char *)strBuf, sizeof(_exomark) - 1, META_MARK);
	if (strncmp(strBuf, _exomark, sizeof(_exomark) - 1) || reset)
		defaults();

	return;
}

void metaData::write(u8* write_buffer, u16 srcBytes, u8 element) {
	metaDataEntry* meta_info = 0;

	//TODO - do not write if the data already there is identical...

	switch (element) {
	case META_CIK:
		if (srcBytes > CIKSize) return;
		writeMetaItem(write_buffer, srcBytes, (int)meta_info->cik); //store CIK
		break;
	case META_SERVER:
		if (srcBytes > serverIPSize) return;
		writeMetaItem(write_buffer, srcBytes, (int)meta_info->server); //store server IP
		break;
	case META_MARK:
		if (srcBytes > markSize) return;
		writeMetaItem(write_buffer, srcBytes, (int)meta_info->mark); //store exosite mark
		break;
	case META_UUID:
		if (srcBytes > UUIDSize) return;
		writeMetaItem(write_buffer, srcBytes, (int)meta_info->uuid); //store UUID
		break;
	case META_MFR:
		if (srcBytes > MFRSize) return;
		writeMetaItem(write_buffer, srcBytes, (int)meta_info->mfr); //store manufacturing info
		break;
	case META_NONE:
	default:
		break;
	}

	return;
}

void metaData::read(u8* read_buffer, u16 destBytes, u8 element) {
	metaDataEntry* meta_info = 0;

	switch (element) {
	case META_CIK:
		if (destBytes < CIKSize) return;
		readMetaItem(read_buffer, CIKSize, (int)meta_info->cik); //read CIK
		break;
	case META_SERVER:
		if (destBytes < serverIPSize) return;
		readMetaItem(read_buffer, serverIPSize, (int)meta_info->server); //read server IP
		break;
	case META_MARK:
		if (destBytes < markSize) return;
		readMetaItem(read_buffer, markSize, (int)meta_info->mark); //read exosite mark
		break;
	case META_UUID:
		if (destBytes < UUIDSize) return;
		readMetaItem(read_buffer, UUIDSize, (int)meta_info->uuid); //read provisioning UUID
		break;
	case META_MFR:
		if (destBytes < MFRSize) return;
		readMetaItem(read_buffer, MFRSize, (int)meta_info->mfr); //read manufacturing info
		break;
	case META_NONE:
	default:
		break;
	}

	return;
}

void metaData::readMetaItem(u8* pucBuffer, s32 iLength, s32 iOffset) {
	EEPROMRead((uint32_t *)pucBuffer, (uint32_t)(iOffset), (uint32_t)iLength);
}

void metaData::writeMetaItem(u8* pucBuffer, s32 iLength, s32 iOffset) {
	EEPROMProgram((uint32_t *)pucBuffer, (uint32_t)(iOffset), (uint32_t)iLength);
}
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: metadata.hpp =========================
