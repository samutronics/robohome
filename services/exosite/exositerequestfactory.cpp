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

using namespace std;
using namespace service::exosite;

cs8 _requestPartCIKHeader[]		= "X-Exosite-CIK: ";
cs8 _requestPartCIK[]			= "15e4890a88d79261608b194c8217c486c68f0007";
cs8 _requestPartContentLength[]	= "Content-Length: ";
cs8 _requestPartGetURL[]		= "GET /onep:v1/stack/alias?";
cs8 _requestPartHTTP[]			= "  HTTP/1.1\r\n";
cs8 _requestPartHost[]			= "Host: m2.exosite.com\r\n";
cs8 _requestPartAccept[]		= "Accept: application/x-www-form-urlencoded; charset=utf-8\r\n";
cs8 _requestPartContent[]		= "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n";
cs8 _requestPartVendor[]		= "vendor=";
cs8 _requestPartModel[]			= "model=";
cs8 _requestPartSerialNumber[]	= "sn=";
cs8 _requestPartCRLF[]			= "\r\n";

s8 exositeRequestFactory::_exositeProvisionInfo[_serialNumberSize + _modelNameSize + _vendorNameSize];
ExositeStatusCodes exositeRequestFactory::_statusCode;

bool exositeRequestFactory::writeRequest(const std::string& request, std::string& buf) {
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

	buf.clear();
	sendLine(buf, POSTDATA_LINE, "/onep:v1/stack/alias");
	sendLine(buf, HOST_LINE, 0);
	sendLine(buf, CIK_LINE, 0);
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

int exositeRequestFactory::readRequest(const std::string& request, std::string& buf) {
	//
	// Modified by Texas Instruments, DGT, changed buflen from unsigned char to
	// unsigned int. comment out declaration of *pcheck to prevent warnings
	// created by CAJ changes below.
	//
	char bufCIK[41];

	if (!getCIK(bufCIK)) {return false;}

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
	result.clear();
	int http_status = getHTTPStatus(buf);
	if (200 == http_status) {
		result.append(static_cast<s8*>(buf->payload), buf->len);
		if(string::npos == result.find("\r\n\r\n")) {
			result.clear();
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

int exositeRequestFactory::init(cs8* vendor, cs8* model, cu8 if_nbr, u8* pui8MACAddr, s32 reset) {
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

void exositeRequestFactory::setCIK(s8* pCIK) {
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

int exositeRequestFactory::info_assemble(cs8* vendor, cs8* model, cs8* sn) {
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

void exositeRequestFactory::sendLine(std::string& buf, u8 LINE, cs8* payload) {
	switch(LINE) {
	case CIK_LINE:
		buf.append(_requestPartCIKHeader);
		buf.append(_requestPartCIK);
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
