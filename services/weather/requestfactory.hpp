// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _REQUESTFACTORY_HPP_
#define _REQUESTFACTORY_HPP_

#include <string.h>

#include "../projectconfiguration.hpp"

namespace service {
namespace weather {

class weatherRequestFactory {
public: inline weatherRequestFactory();

public: const std::string& request(cs8* location, bool forecast, u32 days);
public: inline void reset();

private: std::string _request;
};

weatherRequestFactory::weatherRequestFactory() {
	_request.reserve(255);
}

inline void weatherRequestFactory::reset() {
	_request.clear();
}

} // weatherTask
} // service

#endif // _REQUESTFACTORY_HPP_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: requestfactory.hpp ========================
