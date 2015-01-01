/*****************************************************************************
*
* exosite.c - Exosite cloud communications.
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

#include "exosite_hal_lwip.h"
#include "exosite_meta.h"

#include <string.h>
//#include <apps/apps.h>
//#include "exosite.h"
//#include <inc/common.h>

#include "utils/ustdlib.h"

#include "exosite.hpp"

using namespace manager::exositeTask;

const char* exosite::_requestPartCIKHeader		= "X-Exosite-CIK: ";
const char* exosite::_requestPartContentLength	= "Content-Length: ";
const char* exosite::_requestPartGetURL			= "GET /onep:v1/stack/alias?";
const char* exosite::_requestPartHTTP			= "  HTTP/1.1\r\n";
const char* exosite::_requestPartHost			= "Host: m2.exosite.com\r\n";
const char* exosite::_requestPartAccept			= "Accept: application/x-www-form-urlencoded; charset=utf-8\r\n";
const char* exosite::_requestPartContent		= "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n";
const char* exosite::_requestPartVendor			= "vendor=";
const char* exosite::_requestPartModel			= "model=";
const char* exosite::_requestPartSerialNumber	= "sn=";
const char* exosite::_requestPartCRLF			= "\r\n";







int exosite::write(char* pbuf, unsigned int bufsize) {
	  int success = 0;
	  int http_status = 0;
	  char bufCIK[41];
	  char strBuf[10];

	  if (!exosite_initialized) {
	    status_code = EXO_STATUS_INIT;
	    return success;
	  }

	  if (!Exosite_GetCIK(bufCIK))
	  {
	    return success;
	  }


	  long sock = connect_to_exosite();
	  if (sock < 0) {
	    status_code = EXO_STATUS_BAD_TCP;
	    return 0;
	  }


	// This is an example write POST...
	//  s.send('POST /onep:v1/stack/alias HTTP/1.1\r\n')
	//  s.send('Host: m2.exosite.com\r\n')
	//  s.send('X-Exosite-CIK: 5046454a9a1666c3acfae63bc854ec1367167815\r\n')
	//  s.send('Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n')
	//  s.send('Content-Length: 6\r\n\r\n')
	//  s.send('temp=2')

	  usprintf((int)bufsize, strBuf, 10); //make a string for length
	  sprintf(strBuf, "%d", 10);

	  sendLine(sock, POSTDATA_LINE, "/onep:v1/stack/alias");
	  sendLine(sock, HOST_LINE, NULL);
	  sendLine(sock, CIK_LINE, bufCIK);
	  sendLine(sock, CONTENT_LINE, NULL);
	  sendLine(sock, LENGTH_LINE, strBuf);
	  exoHAL_SocketSend(sock, pbuf, bufsize);

	  http_status = get_http_status(sock);

	  exoHAL_SocketClose(sock);

	  if (401 == http_status)
	  {
	    status_code = EXO_STATUS_NOAUTH;
	  }
	  if (204 == http_status)
	  {
	    success = 1;
	    status_code = EXO_STATUS_OK;
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
    status_code = EXO_STATUS_INIT;
    return success;
  }

  if (!Exosite_GetCIK(bufCIK))
  {
    return success;
  }


  long sock = connect_to_exosite();
  if (sock < 0) {
    status_code = EXO_STATUS_BAD_TCP;
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

  http_status = get_http_status(sock);
  if (200 == http_status)
  {
    char strBuf[RX_SIZE];
    unsigned char crlf = 0;

    do
    {
      strLen = exoHAL_SocketRecv(sock, strBuf, RX_SIZE);
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
    } while (RX_SIZE == strLen);
  }

  exoHAL_SocketClose(sock);

  if (200 == http_status)
  {
    status_code = EXO_STATUS_OK;
  }
  if (204 == http_status)
  {
    status_code = EXO_STATUS_OK;
  }
  if (401 == http_status)
  {
    status_code = EXO_STATUS_NOAUTH;
  }

  return vlen;
}

int exosite::init(const char* vendor, const char* model, const unsigned char if_nbr, int reset) {
	  char struuid[EXOSITE_SN_MAXLENGTH];
	  unsigned char uuid_len = 0;

	  exosite_meta_init(reset);          //always initialize Exosite meta structure
	  uuid_len = exoHAL_ReadUUID(if_nbr, (unsigned char *)struuid);

	  if (0 == uuid_len)
	  {
	    status_code = EXO_STATUS_BAD_UUID;
	    return 0;
	  }
	  if (strlen(vendor) > EXOSITE_VENDOR_MAXLENGTH)
	  {
	    status_code = EXO_STATUS_BAD_VENDOR;
	    return 0;
	  }
	  if (strlen(model) > EXOSITE_MODEL_MAXLENGTH)
	  {
	    status_code = EXO_STATUS_BAD_MODEL;
	    return 0;
	  }

	  exosite_meta_write((unsigned char *)struuid, uuid_len, META_UUID);

	  // read UUID into 'sn'
	  info_assemble(vendor, model, struuid);

	  exosite_initialized = 1;

	  status_code = EXO_STATUS_OK;

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
	    status_code = EXO_STATUS_INIT;
	    return newcik;
	  }
	  update_m2ip();        //check our IP api to see if the old IP is advertising a new one

	  long sock = connect_to_exosite();
	  if (sock < 0) {
	    status_code = EXO_STATUS_BAD_TCP;
	    return 0;
	  }

	  // Get activation Serial Number
	  length = strlen(exosite_provision_info);
	  itoa(length, strLen, 10); //make a string for length

	  sendLine(sock, POSTDATA_LINE, "/provision/activate");
	  sendLine(sock, HOST_LINE, NULL);
	  sendLine(sock, CONTENT_LINE, NULL);
	  sendLine(sock, LENGTH_LINE, strLen);

	  exoHAL_SocketSend(sock, exosite_provision_info, length);

	  http_status = get_http_status(sock);

	  if (200 == http_status)
	  {
	    char strBuf[RX_SIZE];
	    unsigned char strLen, len;
	    unsigned char cik_len_valid = 0;
	    char *p;
	    unsigned char crlf = 0;
	    unsigned char ciklen = 0;
	    char NCIK[CIK_LENGTH + 1];

	    do
	    {
	      strLen = exoHAL_SocketRecv(sock, strBuf, RX_SIZE);
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
	      if (0 < len && 4 == crlf && CIK_LENGTH > ciklen)
	      {
	        // TODO, be more robust - match Content-Length header value to CIK_LENGTH
	        unsigned char need, part;
	        if (!(cik_len_valid == 1)) // cik length != 40
	        {
	          status_code = EXO_STATUS_CONFLICT;
	          exoHAL_SocketClose(sock);
	          return newcik;
	        }
	        need = CIK_LENGTH - ciklen;
	        part = need < len ? need : len;
	        strncpy(NCIK + ciklen, p, part);
	        ciklen += part;
	      }
	    } while (RX_SIZE == strLen);

	    if (CIK_LENGTH == ciklen)
	    {
	      NCIK[40] = 0;
	      Exosite_SetCIK(NCIK);
	      newcik = 1;
	    }
	  }

	  exoHAL_SocketClose(sock);

	  if (200 == http_status)
	    status_code = EXO_STATUS_OK;
	  if (404 == http_status)
	    status_code = EXO_STATUS_BAD_SN;
	  if (409 == http_status || 408 == http_status)
	  {
	    status_code = EXO_STATUS_CONFLICT;
	  }

	  return newcik;
}

void exosite::setCIK(char* pCIK) {
	if (!exosite_initialized) {
		status_code = EXO_STATUS_INIT;
		return;
	}
	exosite_meta_write((unsigned char *)pCIK, CIK_LENGTH, META_CIK);
	status_code = EXO_STATUS_OK;
	return;
}

int exosite::getCIK(char* pCIK) {
	  unsigned char i;
	  char tempCIK[CIK_LENGTH + 1];

	  exosite_meta_read((unsigned char *)tempCIK, CIK_LENGTH, META_CIK);
	  tempCIK[CIK_LENGTH] = 0;

	  for (i = 0; i < CIK_LENGTH; i++)
	  {
	    if (!(tempCIK[i] >= 'a' && tempCIK[i] <= 'f' || tempCIK[i] >= '0' && tempCIK[i] <= '9'))
	    {
	      status_code = EXO_STATUS_BAD_CIK;
	      return 0;
	    }
	  }

	  if (NULL != pCIK)
	    memcpy(pCIK ,tempCIK ,CIK_LENGTH + 1);

	  return 1;
}

int exosite::info_assemble(const char* vendor, const char* model, const char* sn) {
	  int info_len = 0;
	  int assemble_len = 0;
	  char * vendor_info = exosite_provision_info;

	  // verify the assembly length
	  assemble_len = strlen(STR_VENDOR) + strlen(vendor)
	                 + strlen(STR_MODEL) + strlen(model)
	                 + strlen(STR_SN) + 3;
	  if (assemble_len > 95)
	    return info_len;

	  // vendor=
	  memcpy(vendor_info, STR_VENDOR, strlen(STR_VENDOR));
	  info_len = strlen(STR_VENDOR);

	  // vendor="custom's vendor"
	  memcpy(&vendor_info[info_len], vendor, strlen(vendor));
	  info_len += strlen(vendor);

	  // vendor="custom's vendor"&
	  vendor_info[info_len] = '&'; // &
	  info_len += 1;

	  // vendor="custom's vendor"&model=
	  memcpy(&vendor_info[info_len], STR_MODEL, strlen(STR_MODEL));
	  info_len += strlen(STR_MODEL);

	  // vendor="custom's vendor"&model="custom's model"
	  memcpy(&vendor_info[info_len], model, strlen(model));
	  info_len += strlen(model);

	  // vendor="custom's vendor"&model="custom's model"&
	  vendor_info[info_len] = '&'; // &
	  info_len += 1;

	  // vendor="custom's vendor"&model="custom's model"&sn=
	  memcpy(&vendor_info[info_len], STR_SN, strlen(STR_SN));
	  info_len += strlen(STR_SN);

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

	  while (connectRetries++ <= EXOSITE_MAX_CONNECT_RETRY_COUNT) {

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
	      strLen = strlen(STR_CIK_HEADER);
	      memcpy(strBuf,STR_CIK_HEADER,strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      memcpy(&strBuf[strLen],STR_CRLF, 2);
	      strLen += strlen(STR_CRLF);
	      break;
	    case HOST_LINE:
	      strLen = strlen(STR_HOST);
	      memcpy(strBuf,STR_HOST,strLen);
	      break;
	    case CONTENT_LINE:
	      strLen = strlen(STR_CONTENT);
	      memcpy(strBuf,STR_CONTENT,strLen);
	      break;
	    case ACCEPT_LINE:
	      strLen = strlen(STR_ACCEPT);
	      memcpy(strBuf,STR_ACCEPT,strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      break;
	    case LENGTH_LINE: // Content-Length: NN
	      strLen = strlen(STR_CONTENT_LENGTH);
	      memcpy(strBuf,STR_CONTENT_LENGTH,strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      memcpy(&strBuf[strLen],STR_CRLF, 2);
	      strLen += 2;
	      memcpy(&strBuf[strLen],STR_CRLF, 2);
	      strLen += 2;
	      break;
	    case GETDATA_LINE:
	      strLen = strlen(STR_GET_URL);
	      memcpy(strBuf,STR_GET_URL,strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      memcpy(&strBuf[strLen],STR_HTTP, strlen(STR_HTTP));
	      strLen += strlen(STR_HTTP);
	      break;
	    case POSTDATA_LINE:
	      strLen = strlen("POST ");
	      memcpy(strBuf,"POST ", strLen);
	      memcpy(&strBuf[strLen],payload, strlen(payload));
	      strLen += strlen(payload);
	      memcpy(&strBuf[strLen],STR_HTTP, strlen(STR_HTTP));
	      strLen += strlen(STR_HTTP);
	      break;
	    case EMPTY_LINE:
	      strLen = strlen(STR_CRLF);
	      memcpy(strBuf,STR_CRLF,strLen);
	      break;
	    default:
	      break;
	  }

	  strBuf[strLen] = 0;
	  exoHAL_SocketSend(socket, strBuf, strLen);

	  return;
}
