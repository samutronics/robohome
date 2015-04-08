// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			08.04.2015.
//! \note
// =============================================================================
#ifndef _PAIR_HPP_
#define _PAIR_HPP_

#include "projectconfiguration.hpp"

namespace libs {
namespace http {

class pair {
public: inline pair(const std::string& p);
public: inline pair(cu32 k, cu32 value);
public: inline pair(const std::string& k, cu32 value);
public: inline pair(cu32 k, const std::string& value);
public: inline pair(const std::string& k, const std::string& value);

public: inline const std::string& build() const;

private: std::string _p;
private: std::string _key;
private: std::string _value;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline pair::pair(const std::string& p): _p(p) {
	u32 equal = p.find('=');
	if(std::string::npos == equal) {
		_p.clear();
		return;
	}

	_key = p.substr(0, equal - 1);
	_value = p.substr(equal + 1);
}

inline pair::pair(cu32 k, cu32 value) {
	s8 buf[11];
	sprintf(buf, "%d", k);
	_pair += buf;
	_pair += '=';
	sprintf(buf, "%d", value);
	_pair += buf;
}

inline pair::pair(const std::string& k, cu32 value): _pair(k) {
	s8 buf[11];
	sprintf(buf, "%d", value);
	_value += buf;
}

inline pair::pair(cu32 k, const std::string& value): _value(value) {
	s8 buf[11];
	sprintf(buf, "%d", k);
	_key += buf;
}

inline pair::pair(const std::string& k, const std::string& value): _key(k), _value(value) {}

inline const std::string& pair::build() const {
	if(_p.empty()) {
		_p += _key;
		_p += '=';
		_p += _value;
	}

	return _p;
}

} // http
} // libs

#endif // _PAIR_HPP_
// =============================================================================
//! \file
//! \copyright
// ============================ end of file: pair.hpp ==========================
