// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#ifndef _WEB_HPP_
#define _WEB_HPP_

#include "iservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace web {

class web: public IService {
private: enum httpVersion {
		version10 = 0,
		version11 = 1,
		DECLARE_LAST_ENUM(httpVersion)
	};

private: enum httpMethod {
		get = 0,
		post = 1,
		head = 2,
		DECLARE_LAST_ENUM(httpMethod)
	};

	TO_BE_RUNABLE(web)
// =============================================================================
//! \brief Initialize the hardware and necessary objects
//!
//!	Due to the singleton implementation, it will be done whitin the thread context.
//! The initialization consist of the following steps:
// =============================================================================
private: web();

private: httpMethod getHTTPMethodType(const std::string& request) const;
private: bool parseURI(const std::string& request) const;
private: bool parseResource(const std::string& request, cu32 startOfURI, cu32 startOfArguments) const;
private: bool parseDefaultResource(const std::string& request, cu32 startOfURI) const;

// =============================================================================
//! \brief Empty implementation of the task.
//!
//!	This basic implementation is required due to the architecture convenience.
// =============================================================================
private: virtual void task(void *pvParameters);

private: netconn* _connectionFromClient;
};

} // web

} // service

#endif // _WEB_HPP_
// =============================================================================
//! \file
//! \copyright
// =========================== end of file: web.hpp ============================
