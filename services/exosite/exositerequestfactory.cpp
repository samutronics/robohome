// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#include "exosite.hpp"
#include "metadata.hpp"
#include "metadataentry.hpp"
#include "exositerequestfactory.hpp"

using namespace service::exosite;

const s8 exositeRequestFactory::_requestPartCIKHeader[]		= "X-Exosite-CIK: ";
const s8 exositeRequestFactory::_requestPartContentLength[]	= "Content-Length: ";
const s8 exositeRequestFactory::_requestPartGetURL[]		= "GET /onep:v1/stack/alias?";
const s8 exositeRequestFactory::_requestPartHTTP[]			= "  HTTP/1.1\r\n";
const s8 exositeRequestFactory::_requestPartHost[]			= "Host: m2.exosite.com\r\n";
const s8 exositeRequestFactory::_requestPartAccept[]		= "Accept: application/x-www-form-urlencoded; charset=utf-8\r\n";
const s8 exositeRequestFactory::_requestPartContent[]		= "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n";
const s8 exositeRequestFactory::_requestPartVendor[]		= "vendor=";
const s8 exositeRequestFactory::_requestPartModel[]			= "model=";
const s8 exositeRequestFactory::_requestPartSerialNumber[]	= "sn=";
const s8 exositeRequestFactory::_requestPartCRLF[]			= "\r\n";

s8 exositeRequestFactory::_exositeProvisionInfo[_serialNumberSize + _modelNameSize + _vendorNameSize];
ExositeStatusCodes exositeRequestFactory::_statusCode;

bool exositeRequestFactory::write(const std::string& request, std::string& buf) {
	char bufCIK[41];
	char strBuf[10];

	if (!getCIK(bufCIK)) {
		return false;
	}

	// This is an example write POST...
	//  s.send('POST /onep:v1/stack/alias HTTP/1.1\r\n')
	//  s.send('Host: m2.exosite.com\r\n')
	//  s.send('X-Exosite-CIK: 5046454a9a1666c3acfae63bc854ec1367167815\r\n')
	//  s.send('Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n')
	//  s.send('Content-Length: 6\r\n\r\n')
	//  s.send('temp=2')

	sprintf(strBuf, "%d", request.length());

	sendLine(buf, POSTDATA_LINE, "/onep:v1/stack/alias");
	sendLine(buf, HOST_LINE, 0);
	sendLine(buf, CIK_LINE, bufCIK);
	sendLine(buf, CONTENT_LINE, 0);
	sendLine(buf, LENGTH_LINE, strBuf);

	buf.append(request);
	return true;
}

void exositeRequestFactory::parseWriteResult(pbuf* buf) {
	int http_status = getHTTPStatus(buf);
	if (401 == http_status) {
		_statusCode = EXO_STATUS_NOAUTH;
	}
	else if (204 == http_status) {
		_statusCode = EXO_STATUS_OK;
	}
}

int exositeRequestFactory::read(const std::string& request, std::string& buf) {
	//
	// Modified by Texas Instruments, DGT, changed buflen from unsigned char to
	// unsigned int. comment out declaration of *pcheck to prevent warnings
	// created by CAJ changes below.
	//
	char bufCIK[41];

	if (!getCIK(bufCIK)) {
		return false;
	}

	// This is an example read GET
	//  s.send('GET /onep:v1/stack/alias?temp HTTP/1.1\r\n')
	//  s.send('Host: m2.exosite.com\r\n')
	//  s.send('X-Exosite-CIK: 5046454a9a1666c3acfae63bc854ec1367167815\r\n')
	//  s.send('Accept: application/x-www-form-urlencoded; charset=utf-8\r\n\r\n')

	sendLine(buf, GETDATA_LINE, request.c_str());
	sendLine(buf, HOST_LINE, 0);
	sendLine(buf, CIK_LINE, bufCIK);
	sendLine(buf, ACCEPT_LINE, "\r\n");

	return 1;
}

void exositeRequestFactory::parseReadResult(pbuf* buf, std::string& result) {
	int http_status = getHTTPStatus(buf);
	if (200 == http_status) {
		result.clear();
		u8 crlf = 0;
		u32 index = 0;

		// Find 4 consecutive \r or \n - should be: \r\n\r\n
		while (index < buf->len && 4 > crlf) {
			if ('\r' == static_cast<s8*>(buf->payload)[index] || '\n' == static_cast<s8*>(buf->payload)[index]) {
				++crlf;
			}
			else {
				crlf = 0;
			}

			index++;
		}

		// The body is "<key>=<value>"
		if (4 == crlf) {
			// read in the rest of the body as the value
			result.append(static_cast<s8*>(buf->payload) + index, buf->len - index);
		}

		_statusCode = EXO_STATUS_OK;
	}
	else if (204 == http_status) {
		_statusCode = EXO_STATUS_OK;
	}
	else if (401 == http_status) {
		_statusCode = EXO_STATUS_NOAUTH;
	}
}

int exositeRequestFactory::init(const s8* vendor, const s8* model, const u8 if_nbr, u8* pui8MACAddr, s32 reset) {
	char struuid[_serialNumberSize];

	metaData::init(reset);          //always initialize Exosite meta structure

	sprintf((char *)struuid,"%02x%02x%02x%02x%02x%02x",
			(char)pui8MACAddr[0],
			(char)pui8MACAddr[1],
			(char)pui8MACAddr[2],
			(char)pui8MACAddr[3],
			(char)pui8MACAddr[4],
			(char)pui8MACAddr[5]);

	unsigned char uuid_len =  strlen(struuid);

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

	metaData::write((unsigned char *)struuid, uuid_len, META_UUID);

	// read UUID into 'sn'
	info_assemble(vendor, model, struuid);

	_statusCode = EXO_STATUS_OK;

	return 1;
}

void exositeRequestFactory::setCIK(char* pCIK) {
	metaData::write((unsigned char *)pCIK, _CIKSize, META_CIK);
	_statusCode = EXO_STATUS_OK;
	return;
}

int exositeRequestFactory::getCIK(char* pCIK) {
	unsigned char i;
	char tempCIK[_CIKSize + 1];

	metaData::read((unsigned char *)tempCIK, _CIKSize, META_CIK);
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

int exositeRequestFactory::info_assemble(const char* vendor, const char* model, const char* sn) {
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

int exositeRequestFactory::getHTTPStatus(pbuf* buf) {
	char rxBuf[12];

	memcpy(rxBuf, buf->payload, 12);

	// exampel '4','0','4' =>  404  (as number)
	int code = (((rxBuf[9] - 0x30) * 100) +
			((rxBuf[10] - 0x30) * 10) +
			(rxBuf[11] - 0x30));
	return code;
}

void exositeRequestFactory::sendLine(std::string& buf, unsigned char LINE, const char* payload) {
	switch(LINE) {
	case CIK_LINE:
		buf.append(_requestPartCIKHeader);
		buf.append(payload);
		buf.append(_requestPartCRLF);
		break;
	case HOST_LINE:
		buf.append(_requestPartHost);
		break;
	case CONTENT_LINE:
		buf.append(_requestPartContent);
		break;
	case ACCEPT_LINE:
		buf.append(_requestPartAccept);
		buf.append(payload);
		break;
	case LENGTH_LINE: // Content-Length: NN
		buf.append(_requestPartContentLength);
		buf.append(payload);
		buf.append("\r\n\r\n");
		break;
	case GETDATA_LINE:
		buf.append(_requestPartGetURL);
		buf.append(payload);
		buf.append(_requestPartHTTP);
		break;
	case POSTDATA_LINE:
		buf.append("POST ");
		buf.append(payload);
		buf.append(_requestPartHTTP);
		break;
	case EMPTY_LINE:
		buf.append(_requestPartCRLF);
		break;
	default:
		while(true);
	}

	return;
}

// =============================================================================
//! \file
//! \copyright
// ===================== end of file: exositerequestfactory.cpp ================
