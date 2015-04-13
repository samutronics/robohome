// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			12.04.2015.
//! \note
// =============================================================================
#include "postrequest.hpp"

using namespace libs::http;

cs8 postRequest::_contentLengthText[] = "Content-Length: ";

void postRequest::append(argumentPair& p) {
	_content += p.build();

	s8 buf[11];
	sprintf(buf, "%d", _content.size());
	_contentLength.erase(sizeof(_contentLengthText) - 1);
	_contentLength += buf;
	_contentLength += "\r\n\r\n";
}

void postRequest::append(const std::string& str) {
	_content += str;

	s8 buf[11];
	sprintf(buf, "%d", this->_content.size());
	_contentLength.erase(sizeof(_contentLengthText) - 1);
	_contentLength += buf;
	_contentLength += "\r\n\r\n";
}

void postRequest::append(cs8* str, cu32 strSize) {
	(0 == strSize) ? _content.append(str) : _content.append(str, strSize);

	s8 buf[11];
	sprintf(buf, "%d", this->_content.size());
	_contentLength.erase(sizeof(_contentLengthText) - 1);
	_contentLength += buf;
	_contentLength += "\r\n\r\n";
}

// =============================================================================
//! \file
//! \copyright
// ========================== end of file: postrequest.cpp =====================
