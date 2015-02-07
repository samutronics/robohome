// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			26.01.2015.
//! \note
// =============================================================================
#include "abstractclientservice.hpp"

using namespace service;
using namespace systemGlobal;

void abstractclientservice::retryContext(netconn*& connection, s32& error) {
	while(IPGatheringStrategie == IPADDR_USE_DHCP && 0x0 == lwIPLocalIPAddrGet() || 0xFFFFFFFF == lwIPLocalIPAddrGet()) {taskYIELD();}

	// =============================================================================
	//! * At this point, the excecution has to wait for the end of the
	//!		inicialization of the dns module. Until that, argument error will be
	//!		returned from the dns lookup.
	//!	\note Refactoring of each of LwIP services would be nice to became queriable
	//! about its state, or the service send any kind of event.
	// =============================================================================
	ip_addr serverIP;
	error = netconn_gethostbyname(_url, &serverIP);
	if(ERR_OK != error) {return;}

	while(true) {
		// =============================================================================
		//! * Create HTTP get request to query the actual weather informations. It has
		//! to be done only once.
		//! \warning Whitespaces are not allowed in the the request.
		// =============================================================================
		netbuf* request = generateRequest();

		// =============================================================================
		//! .
		//! The further steps are excecuted periodically.
		//! * Connect to the server, and block the execution until the connection will
		//! be established.
		// =============================================================================
		connection = netconn_new(_connectionType);
		if (connection == NULL) {return;}

		error = netconn_connect(connection, &serverIP, _port);
		if (ERR_OK != error) {return;}

		_connectionType == NETCONN_TCP ?
				error = netconn_write(connection, request->p->payload, request->p->tot_len, NETCONN_NOCOPY) :
				error = netconn_send(connection, request);
		if (ERR_OK != error) {return;}

		netbuf* reply = NULL;
		error = netconn_recv(connection, &reply);
		if (ERR_OK != error) {netbuf_delete(reply); return;}

		if(!processingReply(reply)) {return;}

		netconn_close(connection);
		netconn_delete(connection);

		// =============================================================================
		//! * Wait until the time of the next request
		// =============================================================================
		vTaskDelay(_updatePeriode);
	}
}
// =============================================================================
//! \file
//! \copyright
// ================= end of file: abstractclientservice.cpp ====================