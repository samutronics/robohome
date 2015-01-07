// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#ifndef _EXOSITEMANAGER_H_
#define _EXOSITEMANAGER_H_

#include "iservice.hpp"
#include "basicvector.hpp"
#include "../../projectconfiguration.hpp"

namespace service {
namespace exosite {

class exosite: public IService {
private: exosite();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(exosite)

private: static err_t connectToServer();
private: static err_t sendRequest();
private: static void closeConnection(tcp_pcb* psPcb);

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

private: static const u16						_rxTxBufSize = 4096;
private: static basicVector<u8, _rxTxBufSize>	_rxTxBuf;
private: static ip_addr							_serverIP;
private: static tcp_pcb*						_pcb;

private: enum state {
	readRequestSent,
	readRequestProcessed,
	writeRequestSent,
	writeRequestProcessed,
	idle
};

private: static state		_state;
};

} // exosite

} // manager

#endif // _EXOSITEMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.hpp ==========================
