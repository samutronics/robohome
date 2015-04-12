// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.04.2015.
//! \note
// =============================================================================
#ifndef _POSTREQUESTBUILDER_HPP_
#define _POSTREQUESTBUILDER_HPP_

#include "projectconfiguration.hpp"
#include "abstractrequestbuilder.hpp"

namespace libs {
namespace http {

template<ProtocolVersion version = HTTP10> class PostRequestBuilder: public abstractRequestBuilder<version> {
public: inline PostRequestBuilder(cs8* url);
public: inline PostRequestBuilder(const std::string& url);

using abstractRequestBuilder<version>::build;

public: virtual inline void build(argumentPair& p);
public: virtual inline void build(const std::string& p);

private: std::string _contentLength;
private: static cs8 _contentLengthText[];
};

template<ProtocolVersion version> cs8 PostRequestBuilder<version>::_contentLengthText[] = "Content-Length: ";

// =============================================================================
// Inline method implementation
// =============================================================================

template<ProtocolVersion version>
inline PostRequestBuilder<version>::PostRequestBuilder(cs8* url):
abstractRequestBuilder<version>("POST ", url), _contentLength(_contentLengthText) {
	_contentLength += "0\r\n";
	this->_fields.push_back(&this->_version);
	this->_fields.push_back(&this->_headerField);
	this->_fields.push_back(&_contentLength);
	this->_fields.push_back(&this->_content);
}

template<ProtocolVersion version>
inline PostRequestBuilder<version>::PostRequestBuilder(const std::string& url):
abstractRequestBuilder<version>("POST ", url), _contentLength(_contentLengthText) {
	this->_fields.push_back(&this->_version);
	this->_fields.push_back(&this->_headerField);
	this->_fields.push_back(&this->_content);
}

template<ProtocolVersion version>
inline void PostRequestBuilder<version>::build(argumentPair& p) {
	this->_content += p.build();

	s8 buf[11];
	sprintf(buf, "%d", this->_content.size());
	_contentLength.erase(sizeof(_contentLengthText) - 1);
	_contentLength += buf;
	_contentLength += "\r\n\r\n";
}

template<ProtocolVersion version>
inline void PostRequestBuilder<version>::build(const std::string& p) {
	this->_content += p;

	s8 buf[11];
	sprintf(buf, "%d", this->_content.size());
	_contentLength.erase(sizeof(_contentLengthText) - 1);
	_contentLength += buf;
	_contentLength += "\r\n\r\n";
}

} // http
} // libs

#endif // _POSTREQUESTBUILDER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: postrequestbuilder.hpp =================
