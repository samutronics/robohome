// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			12.04.2015.
//! \note
// =============================================================================
#ifndef _RESPONSE_HPP_
#define _RESPONSE_HPP_

#include "message.hpp"
#include "projectconfiguration.hpp"

namespace libs {
namespace http {

class response: public message {
public: inline response(u32 errorCode, const std::string& version);
public: inline response(u32 errorCode, cs8* version, cu32 versionSize = 0);
public: inline response(const std::string& errorCode, const std::string& version);
public: inline response(cs8* errorCode, cu32 errorCodeSize, cs8* version, cu32 versionSize);

public: virtual void append(headerPair& p);
public: virtual void append(argumentPair& p);
public: virtual void append(const std::string& str);
public: virtual void append(cs8* str, cu32 strSize = 0);

private: inline void serializeFields();

private: std::string _error;
private: std::string _contentLength;
private: static cs8 _contentLengthText[];
};

// =============================================================================
// Inline method implementation
// =============================================================================
inline response::response(u32 errorCode, const std::string& version):
				message(version),
				_contentLength(_contentLengthText) {
	s8 buf[11];
	sprintf(buf, "%d", errorCode);
	_error += buf;
	serializeFields();
}

inline response::response(u32 errorCode, cs8* version, cu32 versionSize):
				message(version, versionSize),
				_contentLength(_contentLengthText) {
	s8 buf[11];
	sprintf(buf, "%d", errorCode);
	_error += buf;
	serializeFields();
}

inline response::response(const std::string& errorCode, const std::string& version):
				message(version),
				_error(errorCode),
				_contentLength(_contentLengthText) {
	serializeFields();
}

inline response::response(cs8* errorCode, cu32 errorCodeSize, cs8* version, cu32 versionSize):
				message(version),
				_error(errorCode, errorCodeSize),
				_contentLength(_contentLengthText) {
	serializeFields();
}

inline void response::serializeFields() {
	_fields.push_back(&_version);
	_fields.push_back(&_error);
	_fields.push_back(&_headerField);
	_fields.push_back(&_contentLength);
	_fields.push_back(&_content);
}

} // http
} // libs

#endif // _RESPONSE_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: response.hpp ========================
