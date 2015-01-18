// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#ifndef _EXOSITE_H_
#define _EXOSITE_H_

#include "devicerequestFactory.hpp"
#include "../../projectconfiguration.hpp"

namespace service {
namespace exosite {

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

class exositeRequestFactory {
public: bool writeRequest(const std::string& request, std::string& buf);
public: void parseWriteResult(pbuf* buf);
public: int readRequest(const std::string& request, std::string& buf);
public: void parseReadResult(pbuf* buf, std::string& result);

public: int init(const s8* vendor, const s8* model, const u8 if_nbr, u8* pui8MACAddr, s32 reset);
public: static void setCIK(char* pCIK);
public: static int getCIK(char* pCIK);
public: inline ExositeStatusCodes statusCode() {return _statusCode;}

private: static int info_assemble(const char* vendor, const char* model, const char* sn);
private: static int getHTTPStatus(pbuf* buf);
private: void sendLine(std::string& buf, unsigned char LINE, const char* payload);

private: static ExositeStatusCodes _statusCode;

private: static const u8	_vendorNameSize = 20;
private: static const u8	_modelNameSize = 20;
private: static const u8	_serialNumberSize = 25;
private: static const u8	_CIKSize = 40;

private: static s8			_exositeProvisionInfo[_serialNumberSize + _modelNameSize + _vendorNameSize];

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
// ===================== end of file: exositerequestfactory.hpp ================
