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

void abstractclientservice::task(void* pvParameters) {
	netconn* connection = NULL;
	s32 error = ERR_OK;
	while(true) {
		retryContext(connection, error);
		if(!connection) {
			UARTprintf("%s: Out of memory, retry later\n", _url);
		}
		else if(error != ERR_OK) {
			UARTprintf("%s: Error occured: %d\n", _url, error);
			netconn_close(connection);
			netconn_delete(connection);
			connection = NULL;
		}
		else {
			UARTprintf("%s: Undefined error occured\n", _url);
		}

		vTaskDelay(5000);
	}
}

void abstractclientservice::retryContext(netconn*& connection, s32& error) {
	// =============================================================================
	//! * Try to resolve the URL. If the lookup failed, return with the LWIP's error code.
	// =============================================================================
	ip_addr serverIP;
	error = netconn_gethostbyname(_url, &serverIP);
	if(ERR_OK != error) {return;}

	while(true) {
		// =============================================================================
		//! .
		//! The further steps are excecuted periodically.
		//! * Get the request to be sent from the concrete implementation.
		// =============================================================================
		netbuf* request = generateRequest();

		// =============================================================================
		//! * Create a new connection with the service specified type. The netconn API's
		//!		method only fails in case of out of memory. It's signed with empty pointer
		// =============================================================================
		connection = netconn_new(_connectionType);
		if (connection == NULL) {return;}

		// =============================================================================
		//! * Establish a connection to the server. If it fails, return with the LwIP's error code.
		// =============================================================================
		error = netconn_connect(connection, &serverIP, _port);
		if (ERR_OK != error) {return;}

		// =============================================================================
		//! * Send the request via the appropriate interface and delete the request.
		//!		If the send fails, return with the LwIP's error code.
		// =============================================================================
		_connectionType == NETCONN_TCP ?
				error = netconn_write(connection, request->p->payload, request->p->tot_len, NETCONN_NOCOPY) :
				error = netconn_send(connection, request);
		netbuf_delete(request);
		if (ERR_OK != error) {return;}

		// =============================================================================
		//! * Wait for receive data. If it fails, return with the LwIP's error code.
		//!	\warning Timeout isn't support yet, so the threan can blocked forever,
		//!		it depends on the LwIP's internal.
		// =============================================================================
		netbuf* reply = NULL;
		error = netconn_recv(connection, &reply);
		if (ERR_OK != error) {netbuf_delete(reply); return;}

		// =============================================================================
		//! * Send the received data to the concrate implementation for processing.
		//!		If the implementation was unable to process the reply, return from the
		//!		method. It will be the undefined errors.
		// =============================================================================
		if(!processingReply(reply)) {return;}

		// =============================================================================
		//! * Clean up the resources.
		// =============================================================================
		netconn_close(connection);
		netconn_delete(connection);

		// =============================================================================
		//! * Block the thread until the timout is expired.
		// =============================================================================
		vTaskDelay(_updatePeriode);
	}
}
// =============================================================================
//! \file
//! \copyright
// ================= end of file: abstractclientservice.cpp ====================
