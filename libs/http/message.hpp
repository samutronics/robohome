// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			12.04.2015.
//! \note
// =============================================================================
#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include "pair.hpp"
#include "projectconfiguration.hpp"

namespace libs {
namespace http {

enum ProtocolVersion {
	HTTPUnknown,
	HTTP10,
	HTTP11
};

class message {
public: inline message(const std::string& version);
public: inline message(cs8* version, cu32 versionSize = 0);

public: virtual void append(headerPair& p) = 0;
public: virtual void append(argumentPair& p) = 0;
public: virtual void append(const std::string& str) = 0;
public: virtual void append(cs8* str, cu32 strSize = 0) = 0;

public: inline void trace() const;
public: inline bool next();
public: inline void reset();
public: inline const std::string& current() const;
public: inline void clearArgs();

protected: std::string						_headerField;
protected: std::string						_content;
protected: const std::string				_version;
protected: std::vector<const std::string*>	_fields;
protected: s32								_index;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline message::message(const std::string& version):
				_content(128, 0),
				_version(version),
				_fields(),
				_index(-1) {}

inline message::message(cs8* version, cu32 versionSize):
								_content(128, 0),
								_version(version, versionSize),
								_fields(),
								_index(-1) {}

inline void message::clearArgs() {
	_content.clear();
}

inline void message::reset() {
	_index = -1;
}

inline bool message::next() {
	if(_index < _fields.size() - 1) {
		++_index;
		return true;
	}
	else {
		return false;
	}
}

inline const std::string& message::current() const {
		return *_fields[_index];
}

inline void message::trace() const {
	for(u32 index = 0; index < _fields.size(); index++) {
		UARTwrite(_fields[index]->c_str(), _fields[index]->size());
	}
}

} // http
} // libs

#endif // _MESSAGE_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: message.hpp ===========================
