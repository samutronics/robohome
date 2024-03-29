// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _REQUESTFACTORY_HPP_
#define _REQUESTFACTORY_HPP_

#include "projectconfiguration.hpp"

namespace service {
namespace weather {

class weatherRequestFactory {
public: inline weatherRequestFactory();

public: const std::string& request(cs8* location, cu32 days);
public: inline void reset();

private: std::string _request;
};

inline weatherRequestFactory::weatherRequestFactory(): _request(255, 0) {
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
