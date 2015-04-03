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
private: bool parseArgs(const std::string& request, cu32 startOfArguments) const;
private: bool parseResource(const std::string& request, cu32 startOfURI) const;

private: void makeHttpHeader(std::string& header, cu32 lenght) const;

private: inline FRESULT readResource(cs8* path, u8*& buffer, u32& size) const;

// =============================================================================
//! \brief Empty implementation of the task.
//!
//!	This basic implementation is required due to the architecture convenience.
// =============================================================================
private: virtual void task(void *pvParameters);

private: netconn* _connectionFromClient;
};

// =============================================================================
// Inline method implementation
// =============================================================================

inline FRESULT web::readResource(cs8* path, u8*& buffer, u32& size) const {
	static FIL file;
	FRESULT error = f_open(&file, path, FA_READ);
	if(FR_OK != error) {return error;}

	buffer = new u8[f_size(&file)];
	if(!buffer) {return FR_INT_ERR;}

	size = 0;
	error = f_read(&file, buffer, f_size(&file), &size);
	if(FR_OK != error) {return error;}

	error = f_close(&file);
	return error;
}

} // web
} // service

#endif // _WEB_HPP_
// =============================================================================
//! \file
//! \copyright
// =========================== end of file: web.hpp ============================
