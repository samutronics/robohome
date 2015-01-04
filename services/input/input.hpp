// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "iservice.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace inboundTask {

class input: public IService {
private: input();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(input)
DEFINE_TH
};

}  // inboundTask

}  // service

#endif //_INPUT_H_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: input.hpp =============================
