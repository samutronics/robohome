// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.04.2015.
//! \note
// =============================================================================
#ifndef _ABSTRACTREQUESTBUILDER_HPP_
#define _ABSTRACTREQUESTBUILDER_HPP_

#include "projectconfiguration.hpp"
#include "abstractmessagebuilder.hpp"

namespace libs {
namespace http {

template<ProtocolVersion version> class abstractRequestBuilder: public abstractMessageBuilder<version> {
public: inline abstractRequestBuilder(cs8* method, cs8* url);
public: inline abstractRequestBuilder(cs8* method, const std::string& url);
public: inline abstractRequestBuilder(const std::string& method, cs8* url);
public: inline abstractRequestBuilder(const std::string& method, const std::string& url);

public: virtual void build(argumentPair& p) = 0;
public: virtual void build(headerPair& p) = 0;
public: virtual void build(const std::string& p) = 0;

protected: const std::string _url;
protected: const std::string _method;
};

// =============================================================================
// Inline method implementation
// =============================================================================

template<ProtocolVersion version>
inline abstractRequestBuilder<version>::abstractRequestBuilder(cs8* method, cs8* url): abstractMessageBuilder<version>(), _url(url), _method(method) {
	this->_fields.push_back(&_method);
	this->_fields.push_back(&_url);
}

template<ProtocolVersion version>
inline abstractRequestBuilder<version>::abstractRequestBuilder(cs8* method, const std::string& url): abstractMessageBuilder<version>(), _url(url), _method(method) {
	this->_fields.push_back(&_method);
	this->_fields.push_back(&_url);
}

template<ProtocolVersion version>
inline abstractRequestBuilder<version>::abstractRequestBuilder(const std::string& method, cs8* url): _url(url), _method(method) {
	this->_fields.push_back(&_method);
	this->_fields.push_back(&_url);
}

template<ProtocolVersion version>
inline abstractRequestBuilder<version>::abstractRequestBuilder(const std::string& method, const std::string& url): _url(url), _method(method) {
	this->_fields.push_back(&_method);
	this->_fields.push_back(&_url);
}

} // http
} // libs

#endif // _ABSTRACTREQUESTBUILDER_HPP_
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: abstractrequestbuilder.hpp ===============
