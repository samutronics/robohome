// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _WEATHERMANAGER_H_
#define _WEATHERMANAGER_H_

#include "imanager.hpp"
#include "weatherreport.hpp"
#include "weatherrequestfactory.hpp"
#include "../projectconfiguration.hpp"

namespace manager {
namespace weatherTask {

class weatherManager: public IManager {
private: weatherManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(weatherManager)

private: static err_t connectToServer();
private: static err_t sendRequest();

// =============================================================================
// Callback methods
// =============================================================================

private: static void resolveHostCallback(const char *pcName, struct ip_addr *psIPAddr, void *vpArg);
private: static err_t connectToServerCallback(void *pvArg, struct tcp_pcb *psPcb, err_t iErr);

private: static err_t TCPReceiveCallback(void *pvArg, struct tcp_pcb *psPcb, struct pbuf *psBuf, err_t iErr);
private: static err_t TCPSentCallback(void *pvArg, struct tcp_pcb *psPcb, u16_t ui16Len);
private: static void TCPErrorCallback(void *vPArg, err_t iErr);

// =============================================================================
// Member declarations
// =============================================================================

private: static report			_report;
private: static ip_addr			_serverIP;
private: static tcp_pcb*		_pcb;
private: static weatherRequest	_request;
};

} // weatherTask

} // manager

#endif // _WEATHERMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: weathermanager.hpp ========================