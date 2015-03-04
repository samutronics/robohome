// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			06.12.2014.
//! \note
// =============================================================================
#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "iservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace outbound {

class output: public IService{
private: output();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(output)
DEFINE_TH
};

} // outbound

} // service

#endif // _OUTPUT_H_
// =============================================================================
//! \file
//! \copyright
// ========================== end of file: output.hpp ==========================
