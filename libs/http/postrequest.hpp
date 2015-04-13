// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			12.04.2015.
//! \note
// =============================================================================
#ifndef _POSTREQUEST_HPP_
#define _POSTREQUEST_HPP_

#include "request.hpp"
#include "projectconfiguration.hpp"

namespace libs {
namespace http {

class postRequest: public request {
public: inline postRequest(cs8* url, cu32 urlLength, const std::string& version);
public: inline postRequest(cs8* url, cu32 urlLength, cs8* version, cu32 versionLength);
public: inline postRequest(const std::string& url, const std::string& version);
public: inline postRequest(const std::string& url, cs8* version, cu32 versionLength);

using request::append;

public: virtual void append(argumentPair& p);
public: virtual void append(const std::string& str);
public: virtual void append(cs8* str, cu32 strSize = 0);

private: std::string _contentLength;
private: static cs8 _contentLengthText[];
};

// =============================================================================
// Inline method implementation
// =============================================================================
inline postRequest::postRequest(cs8* url, cu32 urlLength, const std::string& version):
		request("POST ", url, urlLength, version),
		_contentLength(_contentLengthText) {
	_contentLength += "0\r\n";
	_fields.push_back(&_version);
	_fields.push_back(&_headerField);
	_fields.push_back(&_contentLength);
	_fields.push_back(&_content);
}

inline postRequest::postRequest(cs8* url, cu32 urlLength, cs8* version, cu32 versionLength):
		request("POST ", url, urlLength, version, versionLength),
		_contentLength(_contentLengthText) {
	_contentLength += "0\r\n";
	_fields.push_back(&_version);
	_fields.push_back(&_headerField);
	_fields.push_back(&_contentLength);
	_fields.push_back(&_content);
}

inline postRequest::postRequest(const std::string& url, const std::string& version):
		request("POST ", url, version),
		_contentLength(_contentLengthText) {
	_contentLength += "0\r\n";
	_fields.push_back(&_version);
	_fields.push_back(&_headerField);
	_fields.push_back(&_contentLength);
	_fields.push_back(&_content);
}

inline postRequest::postRequest(const std::string& url, cs8* version, cu32 versionLength):
		request("POST ", url, version, versionLength),
		_contentLength(_contentLengthText) {
	_contentLength += "0\r\n";
	_fields.push_back(&_version);
	_fields.push_back(&_headerField);
	_fields.push_back(&_contentLength);
	_fields.push_back(&_content);
}

} // http
} // libs

#endif // _POSTREQUEST_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: postrequest.cpp =====================
