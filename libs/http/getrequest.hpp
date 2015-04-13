// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.04.2015.
//! \note
// =============================================================================
#ifndef _GETREQUEST_HPP_
#define _GETREQUEST_HPP_

#include "request.hpp"
#include "projectconfiguration.hpp"

namespace libs {
namespace http {

class getRequest: public request {
public: inline getRequest(cs8* url, cu32 urlLength, const std::string& version);
public: inline getRequest(cs8* url, cu32 urlLength, cs8* version, cu32 versionLength);
public: inline getRequest(const std::string& url, const std::string& version);
public: inline getRequest(const std::string& url, cs8* version, cu32 versionLength);

using request::append;

public: virtual void append(argumentPair& p);
public: virtual void append(const std::string& str);
public: virtual void append(cs8* str, cu32 strSize = 0);
};

// =============================================================================
// Inline method implementation
// =============================================================================
inline getRequest::getRequest(cs8* url, cu32 urlLength, const std::string& version): request("GET ", url, urlLength, version) {
	_fields.push_back(&_content);
	_fields.push_back(&_version);
	_fields.push_back(&_headerField);
}

inline getRequest::getRequest(cs8* url, cu32 urlLength, cs8* version, cu32 versionLength): request("GET ", url, urlLength, version, versionLength) {
	_fields.push_back(&_content);
	_fields.push_back(&_version);
	_fields.push_back(&_headerField);
}

inline getRequest::getRequest(const std::string& url, const std::string& version): request("GET ", url, version) {
	_fields.push_back(&_content);
	_fields.push_back(&_version);
	_fields.push_back(&_headerField);
}

inline getRequest::getRequest(const std::string& url, cs8* version, cu32 versionLength): request("GET ", url, version, versionLength) {
	_fields.push_back(&_content);
	_fields.push_back(&_version);
	_fields.push_back(&_headerField);
}

} // http
} // libs

#endif // _GETREQUEST_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: getrequest.hpp =======================
