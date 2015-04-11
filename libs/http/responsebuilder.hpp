// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			09.04.2015.
//! \note
// =============================================================================
#ifndef _RESPONSEBUILDER_HPP_
#define _RESPONSEBUILDER_HPP_

#include "projectconfiguration.hpp"
#include "abstractmessagebuilder.hpp"

namespace libs {
namespace http {

template<ProtocolVersion version = HTTP10> class ResponseBuilder: public abstractMessageBuilder<version> {
public: inline ResponseBuilder(u32 errorCode);

public: virtual inline void build(const argumentPair& p);
public: virtual inline void build(const headerPair& p);
public: virtual inline void build(const std::string& p);
};

// =============================================================================
// Inline method implementation
// =============================================================================

template<ProtocolVersion version>
inline ResponseBuilder<version>::ResponseBuilder(u32 errorCode) {

}

template<ProtocolVersion version>
inline void ResponseBuilder<version>::build(const argumentPair& p) {

}

template<ProtocolVersion version>
inline void ResponseBuilder<version>::build(const headerPair& p) {

}

template<ProtocolVersion version>
inline void ResponseBuilder<version>::build(const std::string& p) {

}

} // http
} // libs

#endif // _RESPONSEBUILDER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================= end of file: responsebuilder.hpp ====================
