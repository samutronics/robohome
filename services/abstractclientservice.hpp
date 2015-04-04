// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			26.01.2015.
//! \note
// =============================================================================
#ifndef _ASBTRACTCLIENTSERVICE_H_
#define _ASBTRACTCLIENTSERVICE_H_

#include "projectmanager.hpp"
#include "projectconfiguration.hpp"

class netbuf;
class netconn;

namespace service {

class abstractclientservice {
// =============================================================================
//! \brief Assign the necessary connection informations to the service instance
//! \note Keep in mind, that the constructor blocks the task execution until the
//! IP address isn't present in case of DHCP IP address gathering startegie.
// =============================================================================
protected: inline abstractclientservice(cs8* url, u16 port, netconn_type type, u32 updatePeriode);

// =============================================================================
//! \brief Forward the received data to the client for processing.
//!
//! Implement this abstract method for getting received data from the remote server.
//! The implementation has to return true if the processing was sucessfull
//! or false if not.
//! \note The implementation takes over the ownership of the reply pointer, so it's
//!		responsible for freeing the allocated resources to avoid the memory leaks.
// =============================================================================
protected: virtual bool processingReply(netbuf* reply) = 0;

// =============================================================================
//! \brief Generate request that will be transmitted.
//!
//! Implement this abstract method for providing data to be sent to the remote pair.
//! The implementation has to return at least an empty frame. Empty pointer isn't allowed.
//! \note The caller takes over the ownership of the request pointer and the caller will delete it.
// =============================================================================
protected: virtual netbuf* generateRequest() = 0;

// =============================================================================
//! \brief The method is responsible for the error handling.
//!
//! The task - retryContext composition was required the data transmission and
//! the error handling to be separeted. This method is responsible for managing
//!	the error handling.
// =============================================================================
public: void task(void *pvParameters);

// =============================================================================
//! \brief The method is responsible for the data transmission.
//!
//! The task - retryContext composition was required the data transmission and
//! the error handling to be separeted. This method is responsible for managing
//!	the data data transmission. Internaly, the method consist of the following steps:
// =============================================================================
private: void retryContext(netconn*& connection, s32& error);

private: cs8*				_url;
private: cu16				_port;
private: cu32				_updatePeriode;
private: const netconn_type	_connectionType;
};

inline abstractclientservice::abstractclientservice(cs8* url, u16 port, netconn_type type, u32 updatePeriode):
		_url(url),
		_port(port),
		_updatePeriode(updatePeriode),
		_connectionType(type) {
    u32 IP = 0;
    u32 NetMask = 0;
    u32 GateWay = 0;

    manager::project::ProjectManagerFactory::get()->sysConfig().network(static_cast<u32&>(IP), static_cast<u32&>(NetMask), static_cast<u32&>(GateWay));
    while((0 == IP) && ((0xFFFFFFFF == lwIPLocalIPAddrGet()) || (0x0 == lwIPLocalIPAddrGet()))) {taskYIELD();}
//    while((0xFFFFFFFF == lwIPLocalIPAddrGet()) || (0x0 == lwIPLocalIPAddrGet())) {taskYIELD();}
}

} // service

#endif // _ASBTRACTCLIENTSERVICE_H_
// =============================================================================
//! \file
//! \copyright
// =================== end of file: abstractclientservice.hpp ==================
