// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#ifndef _EXOSITE_H_
#define _EXOSITE_H_

#include "lwip/tcp.h"
#include "basicvector.hpp"
#include "requestFactory.hpp"
#include "../../projectconfiguration.hpp"

namespace manager {
namespace exositeTask {

enum UUIDInterfaceTypes {
    IF_WIFI,
    IF_ENET,
    IF_FILE,
    IF_HDD,
    IF_I2C,
    IF_GPRS,
    IF_NONE
};

enum ExositeStatusCodes {
    EXO_STATUS_OK,
    EXO_STATUS_INIT,
    EXO_STATUS_BAD_UUID,
    EXO_STATUS_BAD_VENDOR,
    EXO_STATUS_BAD_MODEL,
    EXO_STATUS_BAD_INIT,
    EXO_STATUS_BAD_TCP,
    EXO_STATUS_BAD_SN,
    EXO_STATUS_CONFLICT,
    EXO_STATUS_BAD_CIK,
    EXO_STATUS_NOAUTH,
    EXO_STATUS_END
};

class exosite {
public: static bool write(const basicVector<u8, requestFactory::requestBufferSize>& request, basicVector<u8, configuration::requestBufferSize>& buf);
public: static void parseWriteResult(pbuf* buf);
public: static int read(const basicVector<u8, requestFactory::requestBufferSize>& request, basicVector<u8, configuration::requestBufferSize>& buf);
public: static void parseReadResult(pbuf* buf, basicVector<u8, requestFactory::requestBufferSize>& result);

public: static int init(const s8* vendor, const s8* model, const u8 if_nbr, u8* pui8MACAddr, s32 reset);
public: static void setCIK(char* pCIK);
public: static int getCIK(char* pCIK);
public: static inline ExositeStatusCodes statusCode() {return _statusCode;}

private: static int info_assemble(const char* vendor, const char* model, const char* sn);
private: static inline void update_m2ip() {return;}
private: static int getHTTPStatus(pbuf* buf);
private: static void sendLine(basicVector<u8, configuration::requestBufferSize>& buf, unsigned char LINE, const char* payload);

private: static ExositeStatusCodes _statusCode;
private: static int exosite_initialized;

private: static const u8	_vendorNameSize = 20;
private: static const u8	_modelNameSize = 20;
private: static const u8	_serialNumberSize = 25;
private: static const u16	_updateInterval = 4000;
private: static const u8	_CIKSize = 40;

private: static const u8	_maxConnectRetryCount = 5;
private: static const u8	_length = _serialNumberSize + _modelNameSize + _vendorNameSize;
private: static const u8	_receiveSize = 50;
private: static const u8	_MACLength = 6;

private: static s8			_exositeProvisionInfo[_length];

private: static const s8	_requestPartCIKHeader[];
private: static const s8	_requestPartContentLength[];
private: static const s8	_requestPartGetURL[];
private: static const s8	_requestPartHTTP[];
private: static const s8	_requestPartHost[];
private: static const s8	_requestPartAccept[];
private: static const s8	_requestPartContent[];
private: static const s8	_requestPartVendor[];
private: static const s8	_requestPartModel[];
private: static const s8	_requestPartSerialNumber[];
private: static const s8	_requestPartCRLF[];

private: enum lineTypes {
  CIK_LINE,
  HOST_LINE,
  CONTENT_LINE,
  ACCEPT_LINE,
  LENGTH_LINE,
  GETDATA_LINE,
  POSTDATA_LINE,
  EMPTY_LINE
};

};

} // exositeTask

} // manager

#endif // _EXOSITE_H_

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.hpp ==========================