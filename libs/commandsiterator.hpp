// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			27.03.2015.
//! \note
// =============================================================================
#ifndef _COMMANDSITERATOR_HPP_
#define _COMMANDSITERATOR_HPP_

#include "../projectconfiguration.hpp"

namespace libs {

class CommandsIterator {
public: inline CommandsIterator(const std::string& cmd);

public: inline bool next();
public: inline void reset();
public: inline cu32 key() const;
public: inline const std::string& value() const;

private: u32 _key;
private: u32 _position;
private: std::string _value;
private: const std::string _command;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline CommandsIterator::CommandsIterator(const std::string& cmd): _key(0), _position(0), _command(cmd) {}

inline bool CommandsIterator::next() {
	u32 pos = _command.find('=', _position);
	if(std::string::npos == pos) {
		return false;
	}

	_key = strtoul(std::string(_command.substr(_position, pos - _position)).c_str(), NULL, 10);

	pos++;
	_position = _command.find('&', pos);
	if(std::string::npos == _position) {
		_position = _command.size();
	}

	_value = _command.substr(pos, _position - pos);
	_position++;
	return true;
}

inline void CommandsIterator::reset() {
	_key = 0;
	_position = 0;
	_value.clear();
}

inline cu32 CommandsIterator::key() const {
	return _key;
}
inline const std::string& CommandsIterator::value() const {
	return _value;
}

} // libs

#endif // _COMMANDSITERATOR_HPP_
// =============================================================================
//! \file
//! \copyright
// ====================== end of file: commandsiterator.hpp ====================
