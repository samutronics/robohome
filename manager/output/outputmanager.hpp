// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.03.2015.
//! \note
// =============================================================================
#ifndef _OUTPUTMANAGER_HPP_
#define _OUTPUTMANAGER_HPP_

#include "output.hpp"
#include "iinform.hpp"
#include "tristateoutput.hpp"
#include "projectmanager.hpp"
#include "singletonfactory.hpp"
#include "commandsiterator.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace output {

class OutputManager: public libs::IInform {
public: inline const std::map<u32, Output*>& outputs() const;
public: inline bool read(cu16 address) const;
public: inline const std::vector<u32>& read() const;

public: virtual bool write(const libs::CommandsIterator& it);
public: virtual bool read(const libs::CommandsIterator& it, std::string& result) const;

public: virtual ~OutputManager();

protected: OutputManager();

private: std::map<u32, Output*>	_output;
private: std::vector<u32>		_data;
};

typedef libs::SingletonFactory<OutputManager>	OutputManagerFactory;

// =============================================================================
// Inline method implementation
// =============================================================================

inline const std::map<u32, Output*>& OutputManager::outputs() const {
	return _output;
}

inline const std::vector<u32>& OutputManager::read() const {
	return _data;
}

inline bool OutputManager::read(cu16 address) const {
	return (_data[address / (_data.size() * sizeof(_data[0]) * 8)] & (1 << (address % sizeof(_data[0]) * 8)));
}

} // output
} // manager

#endif // _OUTPUTMANAGER_HPP_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: outputmanager.hpp =====================
