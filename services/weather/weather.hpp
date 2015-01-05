// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _WEATHER_H_
#define _WEATHER_H_

#include "report.hpp"
#include "iservice.hpp"
#include "requestfactory.hpp"
#include "../projectconfiguration.hpp"

namespace service {
namespace weatherTask {

class weather: public IService {
// =============================================================================
//! \brief There is no tasks assigned to the constructor, but it has to be
//! declared as private.
// =============================================================================
private: weather();
// =============================================================================
//! \brief Manage network connection
//!
//! The task manages the client - server HTTP communication.
// =============================================================================
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(weather)

// =============================================================================
//! \brief Try to establish connection to the server.
//!
//! First of all, clear existing connection, make a new socket, and send a tcp
//! connection request.
// =============================================================================
private: static err_t connectToServer();

// =============================================================================
//! \brief Call tcp write and flush together.
// =============================================================================
private: static err_t sendRequest();

// =============================================================================
//! \brief Completely clear the socket, and try to erase the local socket object
// =============================================================================
private: static void closeConnection(tcp_pcb* psPcb);

// =============================================================================
//! \brief Assign the IP address to the _serverIP
// =============================================================================
private: static void resolveHostCallback(const char *pcName, struct ip_addr *psIPAddr, void *vpArg);

// =============================================================================
//! \brief Finalize the connection request
//!
//! Check the error state, and in case of any error, close the socket. If the
//! connection was established successful, callbacks will be registered in the
//! socket object.
// =============================================================================
private: static err_t connectToServerCallback(void *pvArg, struct tcp_pcb *psPcb, err_t iErr);

// =============================================================================
//! \brief Callback method for data receiving via TCP socket
// =============================================================================
private: static err_t TCPReceiveCallback(void *pvArg, struct tcp_pcb *psPcb, struct pbuf *psBuf, err_t iErr);

// =============================================================================
//! \brief Dummy implementation for notification: TCP packet has been sent
// =============================================================================
private: static err_t TCPSentCallback(void *pvArg, struct tcp_pcb *psPcb, u16_t ui16Len);

// =============================================================================
//! \brief Dummy implementation for notification: Error occured
// =============================================================================
private: static void TCPErrorCallback(void *vPArg, err_t iErr);

// =============================================================================
// Member declarations
// =============================================================================
private: static report			_report;
private: static ip_addr			_serverIP;
private: static tcp_pcb*		_pcb;
private: static weatherRequest	_request;
private: static bool			_tcpRequestReceived;
};

} // weatherTask

} // service

#endif // _WEATHER_H_
// =============================================================================
//! \file
//! \copyright
// ======================== end of file: weather.hpp ===========================
