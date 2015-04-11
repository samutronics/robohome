// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			08.04.2015.
//! \note
// =============================================================================
#ifndef _ABSTRACTMESSAGEBUILDER_HPP_
#define _ABSTRACTMESSAGEBUILDER_HPP_

#include "pair.hpp"
#include "projectconfiguration.hpp"

namespace libs {
namespace http {

enum ProtocolVersion {
	HTTPUnknown,
	HTTP10,
	HTTP11
};

template<ProtocolVersion version> class abstractMessageBuilder {
public: inline abstractMessageBuilder();

public: virtual void build(argumentPair& p) = 0;
public: virtual void build(headerPair& p) = 0;
public: virtual void build(const std::string& p) = 0;

public: inline void parse() const;
public: inline bool next();
public: inline void reset();
public: inline const std::string* current() const;
public: inline void clear(bool soft = true);

protected: std::string						_headerField;
protected: std::string						_content;
protected: const std::string				_version;
protected: std::vector<const std::string*>	_fields;
protected: s32								_index;
};

// =============================================================================
// Inline method implementation
// =============================================================================

template<ProtocolVersion version> inline abstractMessageBuilder<version>::abstractMessageBuilder():
		_fields(),
		_version((version == HTTP10) ? " HTTP/1.0\r\n" : " HTTP/1.1\r\n"),
		_index(-1) {}

template<ProtocolVersion version> inline void abstractMessageBuilder<version>::clear(bool soft) {
	if(soft) {
		_content.clear();
	}
	else {
		for(u32 index = 0; index < _fields.size(); _fields[index++]->clear());
	}
}

template<ProtocolVersion version> inline void abstractMessageBuilder<version>::reset() {
	_index = -1;
}

template<ProtocolVersion version> inline bool abstractMessageBuilder<version>::next() {
	if(_index < _fields.size() - 1) {
		++_index;
		return true;
	}
	else {
		return false;
	}
}

template<ProtocolVersion version> inline const std::string* abstractMessageBuilder<version>::current() const {
	if(_index < _fields.size()) {
		return _fields[_index];
	}
	else {
		return NULL;
	}
}

template<ProtocolVersion version> inline void abstractMessageBuilder<version>::parse() const {
	for(u32 index = 0; index < _fields.size(); index++) {
		UARTwrite(_fields[index]->c_str(), _fields[index]->size());
	}
}

} // http
} // libs

#endif // _ABSTRACTMESSAGEBUILDER_HPP_
// =============================================================================
//! \file
//! \copyright
// ===================== end of file: abstractmessagebuilder.hpp ===============
