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

enum PairType {
	HTTPArgumentPair,
	HTTPHeaderPair
};

template<PairType type = HTTPArgumentPair>class pair {
public: inline pair(cs8* p, cu32 length = 0);
public: inline pair(const std::string& p);
public: inline pair(cu32 k, cu32 value);
public: inline pair(cs8* k, cu32 keyLength, cu32 value);
public: inline pair(cu32 k, cs8* value, cu32 valueLength);
public: inline pair(const std::string& k, cu32 value);
public: inline pair(cu32 k, const std::string& value);
public: inline pair(const std::string& k, const std::string& value);

public: inline const std::string& build();

private: std::string _p;
private: std::string _key;
private: std::string _value;
};

typedef pair<>					argumentPair;
typedef pair<HTTPHeaderPair>	headerPair;
// =============================================================================
// Inline method implementation
// =============================================================================

template<PairType type> inline pair<type>::pair(cs8* p, cu32 length): _p(p, length) {
	u32 equal = p.find((type == HTTPArgumentPair) ? '=' : ": ");
	if(std::string::npos == equal) {
		_p.clear();
		return;
	}

	_key = p.substr(0, equal - 1);
	_value = p.substr(equal + 1);
}

template<PairType type> inline pair<type>::pair(const std::string& p): _p(p) {
	u32 equal = p.find((type == HTTPArgumentPair) ? '=' : ": ");
	if(std::string::npos == equal) {
		_p.clear();
		return;
	}

	_key = p.substr(0, equal - 1);
	_value = p.substr(equal + 1);
}

template<PairType type> inline pair<type>::pair(cu32 k, cu32 value) {
	s8 buf[11];
	sprintf(buf, "%d", k);
	_key += buf;
	sprintf(buf, "%d", value);
	_value += buf;
}

template<PairType type> inline pair<type>::pair(cs8* k, cu32 keyLength, cu32 value): _key(k, keyLength) {
	s8 buf[11];
	sprintf(buf, "%d", value);
	_value += buf;
}

template<PairType type> inline pair<type>::pair(const std::string& k, cu32 value): _key(k) {
	s8 buf[11];
	sprintf(buf, "%d", value);
	_value += buf;
}

template<PairType type> inline pair<type>::pair(cu32 k, cs8* value, cu32 valueLength): _value(value, valueLength) {
	s8 buf[11];
	sprintf(buf, "%d", k);
	_key += buf;
}

template<PairType type> inline pair<type>::pair(cu32 k, const std::string& value): _value(value) {
	s8 buf[11];
	sprintf(buf, "%d", k);
	_key += buf;
}

template<PairType type> inline pair<type>::pair(const std::string& k, const std::string& value): _key(k), _value(value) {}

template<PairType type> inline const std::string& pair<type>::build() {
	if(_p.empty()) {
		_p += _key;
		_p.append((type == HTTPArgumentPair) ? "=" : ": ");
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
