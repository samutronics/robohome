// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.04.2015.
//! \note
// =============================================================================
#ifndef _GETREQUESTBUILDER_HPP_
#define _GETREQUESTBUILDER_HPP_

#include "projectconfiguration.hpp"
#include "abstractrequestbuilder.hpp"

namespace libs {
namespace http {

template<ProtocolVersion version = HTTP10> class GetRequestBuilder: public abstractRequestBuilder<version> {
public: inline GetRequestBuilder(cs8* url);
public: inline GetRequestBuilder(const std::string& url);

using abstractRequestBuilder<version>::build;

public: virtual inline void build(argumentPair& p);
public: virtual inline void build(const std::string& p);
};

// =============================================================================
// Inline method implementation
// =============================================================================

template<ProtocolVersion version>
inline GetRequestBuilder<version>::GetRequestBuilder(cs8* url): abstractRequestBuilder<version>("GET ", url) {
	this->_fields.push_back(&this->_content);
	this->_fields.push_back(&this->_version);
	this->_fields.push_back(&this->_headerField);
}

template<ProtocolVersion version>
inline GetRequestBuilder<version>::GetRequestBuilder(const std::string& url): abstractRequestBuilder<version>("GET ", url) {
	this->_fields.push_back(&this->_content);
	this->_fields.push_back(&this->_version);
	this->_fields.push_back(&this->_headerField);
}

template<ProtocolVersion version>
inline void GetRequestBuilder<version>::build(argumentPair& p) {
	this->_content += this->_content.empty() ? '?' : '&';
	this->_content += p.build();
}

template<ProtocolVersion version>
inline void GetRequestBuilder<version>::build(const std::string& /*p*/) {
}


} // http
} // libs

#endif // _GETREQUESTBUILDER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: getrequestbuilder.hpp ==================
