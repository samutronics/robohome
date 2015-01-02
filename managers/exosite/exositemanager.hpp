// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#ifndef _EXOSITEMANAGER_H_
#define _EXOSITEMANAGER_H_

#include "imanager.hpp"
#include "basicvector.hpp"

namespace manager {
namespace exositeTask {

class exositeManager: public IManager {
private: exositeManager();
private: virtual void task(void *pvParameters);
TO_BE_RUNABLE(exositeManager)

private: static err_t connectToServer();
private: static err_t sendRequest();
private: static void closeConnection();

// =============================================================================
// Callback methods
// =============================================================================

private: static void resolveHostCallback(const char *pcName, struct ip_addr *psIPAddr, void *vpArg);
private: static err_t connectToServerCallback(void *pvArg, struct tcp_pcb *psPcb, err_t iErr);

private: static err_t TCPReceiveCallback(void *pvArg, struct tcp_pcb *psPcb, struct pbuf *psBuf, err_t iErr);
private: static err_t TCPSentCallback(void *pvArg, struct tcp_pcb *psPcb, u16_t ui16Len);
private: static void TCPErrorCallback(void *vPArg, err_t iErr);

// =============================================================================
// HAL methods
// =============================================================================

public: static int exositeReadCallback(char* palias, char* pbuf, unsigned int buflen);

private: static void socketClose(long lSocket);
private: static u8 socketSend(long lSocket, s8* buffer, s32 length);
private: static u8 socketRecv(long lSocket, s8* buffer, s32 length);

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

} // exositeTask

} // manager

#endif // _EXOSITEMANAGER_H_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: exositemanager.hpp ========================
