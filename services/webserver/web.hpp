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
// =============================================================================
//! \brief Initialize the hardware and necessary objects
//!
//!	Due to the singleton implementation, it will be done whitin the thread context.
//! The initialization consist of the following steps:
// =============================================================================
private: web();

// =============================================================================
//! \brief Empty implementation of the task.
//!
//!	This basic implementation is required due to the architecture convenience.
// =============================================================================
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(web)
};

} // web

} // service

#endif // _WEB_HPP_
// =============================================================================
//! \file
//! \copyright
// =========================== end of file: web.hpp ============================
