/*****************************************************************************
*
* exosite.h - Exosite library interface header
* Copyright (c) 2013, Exosite LLC
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
* 
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright 
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Exosite LLC nor the names of its contributors may
*       be used to endorse or promote products derived from this software 
*       without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#ifndef EXOSITE_H
#define EXOSITE_H

#include <stdint.h>

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
public: static int write(char* pbuf, unsigned int bufsize);
public: static int read(char* palias, char* pbuf, unsigned int buflen);
public: static int init(const char* vendor, const char* model, const unsigned char if_nbr, int reset);
public: static int activate();
public: static void setCIK(char* pCIK);
public: static int getCIK(char* pCIK);
public: static inline ExositeStatusCodes statusCode() {return _statusCode;}

private: static int info_assemble(const char* vendor, const char* model, const char* sn);
private: static inline void update_m2ip() {return;}
private: static int getHTTPStatus(long socket);
private: static long connectToExosite();
private: static void sendLine(long socket, unsigned char LINE, const char* payload);

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

#endif // EXOSITE_H
