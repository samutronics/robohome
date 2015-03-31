// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.03.2015.
//! \note
// =============================================================================
#ifndef _HTTPPAIR_HPP_
#define _HTTPPAIR_HPP_

#include "../projectconfiguration.hpp"

namespace libs {

class HttpPair {
public: inline HttpPair(cu32 k, cu32 value);
public: inline HttpPair(const std::string& k, cu32 value);
public: inline HttpPair(cu32 k, const std::string& value);
public: inline HttpPair(const std::string& k, const std::string& value);

public: inline const std::string& pair() const;

private: std::string _pair;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline HttpPair::HttpPair(cu32 k, cu32 value) {
	s8 buf[11];
	sprintf(buf, "%d", k);
	_pair += buf;
	_pair += '=';
	sprintf(buf, "%d", value);
	_pair += buf;
}

inline HttpPair::HttpPair(const std::string& k, cu32 value): _pair(k) {
	_pair += '=';
	s8 buf[11];
	sprintf(buf, "%d", value);
	_pair += buf;
}

inline HttpPair::HttpPair(cu32 k, const std::string& value) {
	s8 buf[11];
	sprintf(buf, "%d", k);
	_pair += buf;
	_pair += '=';
	_pair += value;
}

inline HttpPair::HttpPair(const std::string& k, const std::string& value): _pair(k) {
	_pair += '=';
	_pair += value;
}

inline const std::string& HttpPair::pair() const {
	return _pair;
}

} // libs

#endif // _HTTPPAIR_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: httppair.hpp =========================
