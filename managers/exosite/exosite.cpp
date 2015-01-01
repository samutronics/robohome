// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#include "exosite_hal_lwip.h"

#include "exosite.hpp"
#include "metadata.hpp"
#include "metadataentry.hpp"

using namespace manager::exositeTask;

const s8 exosite::_requestPartCIKHeader[]		= "X-Exosite-CIK: ";
const s8 exosite::_requestPartContentLength[]	= "Content-Length: ";
const s8 exosite::_requestPartGetURL[]			= "GET /onep:v1/stack/alias?";
const s8 exosite::_requestPartHTTP[]			= "  HTTP/1.1\r\n";
const s8 exosite::_requestPartHost[]			= "Host: m2.exosite.com\r\n";
const s8 exosite::_requestPartAccept[]			= "Accept: application/x-www-form-urlencoded; charset=utf-8\r\n";
const s8 exosite::_requestPartContent[]			= "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n";
const s8 exosite::_requestPartVendor[]			= "vendor=";
const s8 exosite::_requestPartModel[]			= "model=";
const s8 exosite::_requestPartSerialNumber[]	= "sn=";
const s8 exosite::_requestPartCRLF[]			= "\r\n";

int exosite::write(char* pbuf, unsigned int bufsize) {
	  int success = 0;
	  int http_status = 0;
	  char bufCIK[41];
	  char strBuf[10];

	  if (!exosite_initialized) {
	    _statusCode = EXO_STATUS_INIT;
	    return success;
	  }

	  if (!getCIK(bufCIK)) {
	    return success;
	  }


	  long sock = connectToExosite();
	  if (sock < 0) {
	    _statusCode = EXO_STATUS_BAD_TCP;
	    return 0;
	  }


	// This is an example write POST...
	//  s.send('POST /onep:v1/stack/alias HTTP/1.1\r\n')
	//  s.send('Host: m2.exosite.com\r\n')
	//  s.send('X-Exosite-CIK: 5046454a9a1666c3acfae63bc854ec1367167815\r\n')
	//  s.send('Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n')
	//  s.send('Content-Length: 6\r\n\r\n')
	//  s.send('temp=2')

	  sprintf(strBuf, "%d", bufsize);

	  sendLine(sock, POSTDATA_LINE, "/onep:v1/stack/alias");
	  sendLine(sock, HOST_LINE, NULL);
	  sendLine(sock, CIK_LINE, bufCIK);
	  sendLine(sock, CONTENT_LINE, NULL);
	  sendLine(sock, LENGTH_LINE, strBuf);
	  exoHAL_SocketSend(sock, pbuf, bufsize);

	  http_status = getHTTPStatus(sock);

	  exoHAL_SocketClose(sock);

	  if (401 == http_status) {
	    _statusCode = EXO_STATUS_NOAUTH;
	  }
	  if (204 == http_status) {
	    success = 1;
	    _statusCode = EXO_STATUS_OK;
	  }

	  return success;
}

int exosite::read(char* palias, char* pbuf, unsigned int buflen) {
    //
    // Modified by Texas Instruments, DGT, changed buflen from unsigned char to
    // unsigned int. comment out declaration of *pcheck to prevent warnings
    // created by CAJ changes below.
    //
  int success = 0;
  int http_status = 0;
  char bufCIK[41];
  unsigned char strLen, len, vlen;
  char *p;
  //char *pcheck;

  if (!exosite_initialized) {
    _statusCode = EXO_STATUS_INIT;
    return success;
  }

  if (!getCIK(bufCIK))
  {
    return success;
  }


  long sock = connectToExosite();
  if (sock < 0) {
    _statusCode = EXO_STATUS_BAD_TCP;
    return 0;
  }

// This is an example read GET
//  s.send('GET /onep:v1/stack/alias?temp HTTP/1.1\r\n')
//  s.send('Host: m2.exosite.com\r\n')
//  s.send('X-Exosite-CIK: 5046454a9a1666c3acfae63bc854ec1367167815\r\n')
//  s.send('Accept: application/x-www-form-urlencoded; charset=utf-8\r\n\r\n')

  sendLine(sock, GETDATA_LINE, palias);
  sendLine(sock, HOST_LINE, NULL);
  sendLine(sock, CIK_LINE, bufCIK);
  sendLine(sock, ACCEPT_LINE, "\r\n");

  //
  // Modified by Texas Instruments DGT comment reference to pcheck no longer
  // used. See TI CAJ modification below.
  //
  //pcheck = palias;
  vlen = 0;

  http_status = getHTTPStatus(sock);
  if (200 == http_status)
  {
    char strBuf[_receiveSize];
    unsigned char crlf = 0;

    do
    {
      strLen = exoHAL_SocketRecv(sock, strBuf, _receiveSize);
      len = strLen;
      p = strBuf;

      // Find 4 consecutive \r or \n - should be: \r\n\r\n
      while (0 < len && 4 > crlf)
      {
        if ('\r' == *p || '\n' == *p)
        {
          ++crlf;
        }
        else
        {
          crlf = 0;
        }
        ++p;
        --len;
      }

      // The body is "<key>=<value>"
      if (0 < len && 4 == crlf && buflen > vlen)
      {

          // Code below removed by CAJ. Removing the key works for a single
          // READ request, but doesn't work if multiple values were requested.
          // For multiple values, the server is not guaranteed to return every
          // value in the same order that they were sent. This means that the
          // caller will need the "key" to be able to determine which value
          // belongs with which alias.

        // Move past "<key>"
//        while (0 < len && 0 != *pcheck)
//        {
//          if (*pcheck == *p)
//          {
//            ++pcheck;
//          }
//          else
//          {
//            pcheck = palias;
//          }
//          ++p;
//          --len;
//        }
//
//        // Match '=',  we should now have '<key>='
//        if (0 < len && 0 == *pcheck && '=' == *p)
//        {
//          ++p;
//          --len;
//        }
//
        // read in the rest of the body as the value
        while (0 < len && buflen > vlen)
        {
          pbuf[vlen++] = *p++;
          --len;
        }
      }
    } while (_receiveSize == strLen);
  }

  exoHAL_SocketClose(sock);

  if (200 == http_status) {
    _statusCode = EXO_STATUS_OK;
  }
  if (204 == http_status) {
    _statusCode = EXO_STATUS_OK;
  }
  if (401 == http_status) {
    _statusCode = EXO_STATUS_NOAUTH;
  }

  return vlen;
}

int exosite::init(const char* vendor, const char* model, const unsigned char if_nbr, int reset) {
	  char struuid[_serialNumberSize];
	  unsigned char uuid_len = 0;

	  exosite_meta_init(reset);          //always initialize Exosite meta structure
	  uuid_len = exoHAL_ReadUUID(if_nbr, (unsigned char *)struuid);

	  if (0 == uuid_len) {
	    _statusCode = EXO_STATUS_BAD_UUID;
	    return 0;
	  }
	  if (strlen(vendor) > _vendorNameSize) {
	    _statusCode = EXO_STATUS_BAD_VENDOR;
	    return 0;
	  }
	  if (strlen(model) > _modelNameSize) {
	    _statusCode = EXO_STATUS_BAD_MODEL;
	    return 0;
	  }

	  exosite_meta_write((unsigned char *)struuid, uuid_len, META_UUID);

	  // read UUID into 'sn'
	  info_assemble(vendor, model, struuid);

	  exosite_initialized = 1;

	  _statusCode = EXO_STATUS_OK;

	  return 1;
}

int exosite::activate() {
	  int length;
	  char strLen[5];
	 // char cmp_ss[18] = "Content-Length: 40";
	  char *cmp_ss = "Content-Length: 40";
	  char *cmp = cmp_ss;
	  int newcik = 0;
	  int http_status = 0;

	  if (!exosite_initialized) {
	    _statusCode = EXO_STATUS_INIT;
	    return newcik;
	  }
	  update_m2ip();        //check our IP api to see if the old IP is advertising a new one

	  long sock = connectToExosite();
	  if (sock < 0) {
	    _statusCode = EXO_STATUS_BAD_TCP;
	    return 0;
	  }

	  // Get activation Serial Number
	  length = strlen(_exositeProvisionInfo);
	  sprintf(strLen, "%d", length);

	  sendLine(sock, POSTDATA_LINE, "/provision/activate");
	  sendLine(sock, HOST_LINE, NULL);
	  sendLine(sock, CONTENT_LINE, NULL);
	  sendLine(sock, LENGTH_LINE, strLen);

	  exoHAL_SocketSend(sock, _exositeProvisionInfo, length);

	  http_status = getHTTPStatus(sock);

	  if (200 == http_status)
	  {
	    char strBuf[_receiveSize];
	    unsigned char strLen, len;
	    unsigned char cik_len_valid = 0;
	    char *p;
	    unsigned char crlf = 0;
	    unsigned char ciklen = 0;
	    char NCIK[_CIKSize + 1];

	    do
	    {
	      strLen = exoHAL_SocketRecv(sock, strBuf, _receiveSize);
	      len = strLen;
	      p = strBuf;

	      // Find 4 consecutive \r or \n - should be: \r\n\r\n
	      while (0 < len && 4 > crlf)
	      {
	        if ('\r' == *p || '\n' == *p)
	        {
	          ++crlf;
	        }
	        else
	        {
	          crlf = 0;
	          if (*cmp == *p)
	          {
	            // check the cik length from http response
	            cmp++;
	            if (cmp > cmp_ss + strlen(cmp_ss) - 1)
	              cik_len_valid = 1;
	          }
	          else
	            cmp = cmp_ss;
	        }
	        ++p;
	        --len;
	      }

	      // The body is the cik
	      if (0 < len && 4 == crlf && _CIKSize > ciklen)
	      {
	        // TODO, be more robust - match Content-Length header value to CIK_LENGTH
	    	  unsigned char need, part;
	    	  if (!(cik_len_valid == 1)) // cik length != 40
	    	  {
	    		  _statusCode = EXO_STATUS_CONFLICT;
	    		  exoHAL_SocketClose(sock);
	    		  return newcik;
	    	  }
	    	  need = _CIKSize - ciklen;
	        part = need < len ? need : len;
	        strncpy(NCIK + ciklen, p, part);
	        ciklen += part;
	      }
	    } while (_receiveSize == strLen);

	    if (_CIKSize == ciklen)
	    {
	      NCIK[40] = 0;
	      setCIK(NCIK);
	      newcik = 1;
	    }
	  }

	  exoHAL_SocketClose(sock);

	  if (200 == http_status) {
		  _statusCode = EXO_STATUS_OK;
	  }
	  else if (404 == http_status) {
		  _statusCode = EXO_STATUS_BAD_SN;
	  }
	  else if (409 == http_status || 408 == http_status) {
		  _statusCode = EXO_STATUS_CONFLICT;
	  }

	  return newcik;
}

void exosite::setCIK(char* pCIK) {
	if (!exosite_initialized) {
		_statusCode = EXO_STATUS_INIT;
		return;
	}
	exosite_meta_write((unsigned char *)pCIK, _CIKSize, META_CIK);
	_statusCode = EXO_STATUS_OK;
	return;
}

int exosite::getCIK(char* pCIK) {
	  unsigned char i;
	  char tempCIK[_CIKSize + 1];

	  exosite_meta_read((unsigned char *)tempCIK, _CIKSize, META_CIK);
	  tempCIK[_CIKSize] = 0;

	  for (i = 0; i < _CIKSize; i++)
	  {
	    if (!(tempCIK[i] >= 'a' && tempCIK[i] <= 'f' || tempCIK[i] >= '0' && tempCIK[i] <= '9'))
	    {
	      _statusCode = EXO_STATUS_BAD_CIK;
	      return 0;
	    }
	  }

	  if (NULL != pCIK)
	    memcpy(pCIK, tempCIK, _CIKSize + 1);

	  return 1;
}

int exosite::info_assemble(const char* vendor, const char* model, const char* sn) {
	  int info_len = 0;
	  int assemble_len = 0;
	  char * vendor_info = _exositeProvisionInfo;

	  // verify the assembly length
	  assemble_len = strlen(_requestPartVendor) + strlen(vendor)
	                 + strlen(_requestPartModel) + strlen(model)
	                 + strlen(_requestPartSerialNumber) + 3;
	  if (assemble_len > 95)
	    return info_len;

	  // vendor=
	  memcpy(vendor_info, _requestPartVendor, strlen(_requestPartVendor));
	  info_len = strlen(_requestPartVendor);

	  // vendor="custom's vendor"
	  memcpy(&vendor_info[info_len], vendor, strlen(vendor));
	  info_len += strlen(vendor);

	  // vendor="custom's vendor"&
	  vendor_info[info_len] = '&'; // &
	  info_len += 1;

	  // vendor="custom's vendor"&model=
	  memcpy(&vendor_info[info_len], _requestPartModel, strlen(_requestPartModel));
	  info_len += strlen(_requestPartModel);

	  // vendor="custom's vendor"&model="custom's model"
	  memcpy(&vendor_info[info_len], model, strlen(model));
	  info_len += strlen(model);

	  // vendor="custom's vendor"&model="custom's model"&
	  vendor_info[info_len] = '&'; // &
	  info_len += 1;

	  // vendor="custom's vendor"&model="custom's model"&sn=
	  memcpy(&vendor_info[info_len], _requestPartSerialNumber, strlen(_requestPartSerialNumber));
	  info_len += strlen(_requestPartSerialNumber);

	  // vendor="custom's vendor"&model="custom's model"&sn="device's sn"
	  memcpy(&vendor_info[info_len], sn, strlen(sn));
	  info_len += strlen(sn);

	  vendor_info[info_len] = 0;

	  return info_len;
}

int exosite::getHTTPStatus(long socket) {
	  char rxBuf[12];
	  int rxLen = 0;
	  int code = 0;

	  rxLen = exoHAL_SocketRecv(socket, rxBuf, 12);

	  if (12 == rxLen)
	  {
	    // exampel '4','0','4' =>  404  (as number)
	    code = (((rxBuf[9] - 0x30) * 100) +
	            ((rxBuf[10] - 0x30) * 10) +
	            (rxBuf[11] - 0x30));
	    return code;
	  }
	  return 0;
}

long exosite::connectToExosite() {
	  unsigned char connectRetries = 0;
	  unsigned char server[META_SERVER_SIZE];
	  long sock = -1;

	  exosite_meta_read(server, META_SERVER_SIZE, META_SERVER);

	  while (connectRetries++ <= _maxConnectRetryCount) {

	    sock = exoHAL_SocketOpenTCP(server);

	    if (sock == -1)
	    {
	      continue;
	    }

	    if (exoHAL_ServerConnect(sock) < 0)  // Try to connect
	    {
	      // TODO - the typical reason the connect doesn't work is because
	      // something was wrong in the way the comms hardware was initialized (timing, bit
	      // error, etc...). There may be a graceful way to kick the hardware
	      // back into gear at the right state, but for now, we just
	      // return and let the caller retry us if they want
	      continue;
	    } else {
	      connectRetries = 0;
	      break;
	    }
	  }

	  // Success
	  return sock;
}

void exosite::sendLine(long socket, unsigned char LINE, const char* payload) {
	  char strBuf[70];
	  unsigned char strLen;

	  switch(LINE) {
	    case CIK_LINE:
	      strLen = sizeof(_requestPartCIKHeader) - 1;
	      memcpy(strBuf,_requestPartCIKHeader,strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      memcpy(&strBuf[strLen],_requestPartCRLF, 2);
	      strLen += sizeof(_requestPartCRLF) - 1;
	      break;
	    case HOST_LINE:
	      strLen = sizeof(_requestPartHost) - 1;
	      memcpy(strBuf,_requestPartHost,strLen);
	      break;
	    case CONTENT_LINE:
	      strLen = sizeof(_requestPartContent) - 1;
	      memcpy(strBuf,_requestPartContent,strLen);
	      break;
	    case ACCEPT_LINE:
	      strLen = sizeof(_requestPartAccept) - 1;
	      memcpy(strBuf,_requestPartAccept,strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      break;
	    case LENGTH_LINE: // Content-Length: NN
	      strLen = sizeof(_requestPartContentLength) - 1;
	      memcpy(strBuf,_requestPartContentLength,strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      memcpy(&strBuf[strLen],_requestPartCRLF, 2);
	      strLen += 2;
	      memcpy(&strBuf[strLen],_requestPartCRLF, 2);
	      strLen += 2;
	      break;
	    case GETDATA_LINE:
	      strLen = sizeof(_requestPartGetURL) - 1;
	      memcpy(strBuf,_requestPartGetURL,strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      memcpy(&strBuf[strLen], _requestPartHTTP, sizeof(_requestPartHTTP) - 1);
	      strLen += sizeof(_requestPartHTTP) - 1;
	      break;
	    case POSTDATA_LINE:
	      strLen = sizeof("POST ") - 1;
	      memcpy(strBuf,"POST ", strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      memcpy(&strBuf[strLen],_requestPartHTTP, sizeof(_requestPartHTTP) - 1);
	      strLen += sizeof(_requestPartHTTP) - 1;
	      break;
	    case EMPTY_LINE:
	      strLen = sizeof(_requestPartCRLF) - 1;
	      memcpy(strBuf,_requestPartCRLF,strLen);
	      break;
	    default:
	      break;
	  }

	  strBuf[strLen] = 0;
	  exoHAL_SocketSend(socket, strBuf, strLen);

	  return;
}

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.cpp ==========================
