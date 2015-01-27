// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			26.01.2015.
//! \note
// =============================================================================
#ifndef _ASBTRACTSERVICEREQUESTTRANSMITTER_H_
#define _ASBTRACTSERVICEREQUESTTRANSMITTER_H_

#include "../projectconfiguration.hpp"

class netbuf;
class netconn;

namespace communication {

class asbtractServiceRequestTransmitter {
protected: inline asbtractServiceRequestTransmitter(const char* url, u16 port, netconn_type type, u32 updatePeriode);

protected: void retryContext(netconn*& connection, s32& error);

protected: virtual bool processingReply(netbuf* reply) = 0;
protected: virtual netbuf* generateRequest() = 0;

private: const char*		_url;
private: const u16			_port;
private: ip_addr			_serverIP;
private: const u32			_updatePeriode;
private: const netconn_type	_connectionType;
};

inline asbtractServiceRequestTransmitter::asbtractServiceRequestTransmitter(const char* url, u16 port, netconn_type type, u32 updatePeriode):
		_url(url),
		_port(port),
		_updatePeriode(updatePeriode),
		_connectionType(type) {}

} // communication

#endif // _ASBTRACTSERVICEREQUESTTRANSMITTER_H_
// =============================================================================
//! \file
//! \copyright
// ============ end of file: asbtractservicerequesttransmitter.hpp =============
