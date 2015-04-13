// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			12.04.2015.
//! \note
// =============================================================================
#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_

#include "message.hpp"
#include "projectconfiguration.hpp"

namespace libs {
namespace http {

class request: public message {
public: inline request(const std::string& method,		const std::string& url,		const std::string& version);
public: inline request(const std::string& method,		const std::string& url,		cs8* version, cu32 versionLength);
public: inline request(const std::string& method,		cs8* url, cu32 urlLength,	const std::string& version);
public: inline request(const std::string& method,		cs8* url, cu32 urlLength,	cs8* version, cu32 versionLength);
public: inline request(cs8* method, cu32 methodLength,	const std::string& url,		const std::string& version);
public: inline request(cs8* method, cu32 methodLength,	const std::string& url,		cs8* version, cu32 versionLength);
public: inline request(cs8* method, cu32 methodLength,	cs8* url, cu32 urlLength,	const std::string& version);
public: inline request(cs8* method, cu32 methodLength,	cs8* url, cu32 urlLength,	cs8* version, cu32 versionLength);

public: virtual void append(headerPair& p);
public: virtual void append(argumentPair& p) = 0;
public: virtual void append(const std::string& str) = 0;
public: virtual void append(cs8* str, cu32 strSize = 0) = 0;

protected: const std::string _url;
protected: const std::string _method;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline request::request(const std::string& method, const std::string& url, const std::string& version):
						message(version),
						_url(url),
						_method(method) {
	_fields.push_back(&_method);
	_fields.push_back(&_url);
}

inline request::request(const std::string& method, const std::string& url, cs8* version, cu32 versionLength):
					message(version, versionLength),
					_url(url),
					_method(method) {
	_fields.push_back(&_method);
	_fields.push_back(&_url);
}

inline request::request(const std::string& method, cs8* url, cu32 urlLength, const std::string& version):
						message(version),
						_url(url, urlLength),
						_method(method) {
	_fields.push_back(&_method);
	_fields.push_back(&_url);
}

inline request::request(const std::string& method, cs8* url, cu32 urlLength, cs8* version, cu32 versionLength):
							message(version, versionLength),
							_url(url, urlLength),
							_method(method) {
	_fields.push_back(&_method);
	_fields.push_back(&_url);
}

inline request::request(cs8* method, cu32 methodLength, const std::string& url, const std::string& version):
										message(version),
										_url(url),
										_method(method, methodLength) {
	_fields.push_back(&_method);
	_fields.push_back(&_url);
}

inline request::request(cs8* method, cu32 methodLength, const std::string& url, cs8* version, cu32 versionLength):
					message(version, versionLength),
					_url(url),
					_method(method, methodLength) {
	_fields.push_back(&_method);
	_fields.push_back(&_url);
}

inline request::request(cs8* method, cu32 methodLength,	cs8* url, cu32 urlLength, const std::string& version):
					message(version),
					_url(url, urlLength),
					_method(method, methodLength) {
	_fields.push_back(&_method);
	_fields.push_back(&_url);
}

inline request::request(cs8* method, cu32 methodLength,	cs8* url, cu32 urlLength, cs8* version, cu32 versionLength):
														message(version, versionLength),
														_url(url, urlLength),
														_method(method, methodLength) {
	_fields.push_back(&_method);
	_fields.push_back(&_url);
}

inline void request::append(headerPair& p) {
	_headerField += p.build();
	_headerField += "\r\n";
}

} // http
} // libs

#endif // _REQUEST_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: request.hpp =========================
