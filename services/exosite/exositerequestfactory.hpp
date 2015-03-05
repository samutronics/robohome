// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			01.01.2015.
//! \note
// =============================================================================
#ifndef _EXOSITEREQUESTFACTORY_H_
#define _EXOSITEREQUESTFACTORY_H_

#include "../../projectconfiguration.hpp"

namespace service {
namespace exosite {

class exositeRequestFactory {
private: enum lineTypes {
  CIK_LINE,
  HOST_LINE,
  CONTENT_LINE,
  ACCEPT_LINE,
  LENGTH_LINE,
  GETDATA_LINE,
  POSTDATA_LINE,
  EMPTY_LINE
};

public: bool writeRequest(const std::string& request, std::string& buf);
public: void parseWriteResult(pbuf* buf);
public: int readRequest(const std::string& request, std::string& buf);
public: void parseReadResult(pbuf* buf, std::string& result);

private: int getHTTPStatus(pbuf* buf);
private: template<lineTypes LINE> void sendLine(std::string& buf, cs8* payload);
};

} // exosite
} // service

#endif // _EXOSITEREQUESTFACTORY_H_

// =============================================================================
//! \file
//! \copyright
// ===================== end of file: exositerequestfactory.hpp ================
