// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			12.04.2015.
//! \note
// =============================================================================
#include "getrequest.hpp"

using namespace libs::http;

void getRequest::append(argumentPair& p) {
	_content += _content.empty() ? '?' : '&';
	_content += p.build();
}

void getRequest::append(const std::string& /*str*/) {}

void getRequest::append(cs8* /*str*/, cu32 /*strSize*/) {}

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: getrequest.cpp =======================
