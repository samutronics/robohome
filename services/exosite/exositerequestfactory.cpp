// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#include "exositerequestfactory.hpp"

using namespace std;
using namespace service::exosite;

cs8 _requestPartCIKHeader[]		= "X-Exosite-CIK: ";
cs8 _requestPartCIK[]			= "4298029770c90c46ca91d91a7106643fd36b7a39";
cs8 _requestPartContentLength[]	= "Content-Length: ";
cs8 _requestPartGetURL[]		= "GET /onep:v1/stack/alias?";
cs8 _requestPartHTTP[]			= "  HTTP/1.1\r\n";
cs8 _requestPartHost[]			= "Host: m2.exosite.com\r\n";
cs8 _requestPartAccept[]		= "Accept: application/x-www-form-urlencoded; charset=utf-8\r\n";
cs8 _requestPartContent[]		= "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n";
cs8 _requestPartCRLF[]			= "\r\n";

void exositeRequestFactory::writeRequest(const std::string& request, std::string& buf) {
	char strBuf[10];

	// This is an example write POST...
	//  s.send('POST /onep:v1/stack/alias HTTP/1.1\r\n')
	//  s.send('Host: m2.exosite.com\r\n')
	//  s.send('X-Exosite-CIK: 5046454a9a1666c3acfae63bc854ec1367167815\r\n')
	//  s.send('Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n')
	//  s.send('Content-Length: 6\r\n\r\n')
	//  s.send('temp=2')

	sprintf(strBuf, "%d", request.length());

	buf.clear();
	sendLine<POSTDATA_LINE>(buf, "/onep:v1/stack/alias");
	sendLine<HOST_LINE>(buf, NULL);
	sendLine<CIK_LINE>(buf, NULL);
	sendLine<CONTENT_LINE>(buf, NULL);
	sendLine<LENGTH_LINE>(buf, strBuf);

	buf.append(request);
}

void exositeRequestFactory::parseWriteResult(pbuf* buf) {
	getHTTPStatus(buf);
}

void exositeRequestFactory::readRequest(const std::string& request, std::string& buf) {
	// This is an example read GET
	//  s.send('GET /onep:v1/stack/alias?temp HTTP/1.1\r\n')
	//  s.send('Host: m2.exosite.com\r\n')
	//  s.send('X-Exosite-CIK: 5046454a9a1666c3acfae63bc854ec1367167815\r\n')
	//  s.send('Accept: application/x-www-form-urlencoded; charset=utf-8\r\n\r\n')

	buf.clear();
	sendLine<GETDATA_LINE>(buf, request.c_str());
	sendLine<HOST_LINE>(buf, NULL);
	sendLine<CIK_LINE>(buf, NULL);
	sendLine<ACCEPT_LINE>(buf, "\r\n");
}

void exositeRequestFactory::parseReadResult(pbuf* buf, std::string& result) {
	result.clear();
	int http_status = getHTTPStatus(buf);
	if (200 == http_status) {
		result.append(static_cast<s8*>(buf->payload), buf->len);
		u32 args = result.find("\r\n\r\n");
		if(args == string::npos) {
			result.clear();
		}

		result.erase(0, args + 4);
	}
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

template<exositeRequestFactory::lineTypes LINE> void exositeRequestFactory::sendLine(std::string& buf, cs8* payload) {
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
}

// =============================================================================
//! \file
//! \copyright
// ===================== end of file: exositerequestfactory.cpp ================
