// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#include "exosite.hpp"
#include "devicestatistic.hpp"
#include "exositerequestfactory.hpp"
#include "../projectconfiguration.hpp"

using namespace std;
using namespace service::exosite;

bool Exosite::processingReply(netbuf* reply) {
	if(_requestPost) {
		_exositeRequestFactory.parseWriteResult(reply->p);
	}
	else {
		_exositeRequestFactory.parseReadResult(reply->p, _workerBuffer);
		deviceStatistic::reset();
		while(deviceStatistic::next()) {
			taskENTER_CRITICAL();
			_deviceRequestFactory.updateEntryByResponse(*deviceStatistic::current(), _workerBuffer);

			//Guard the message printing.
			taskENTER_CRITICAL();
			if(deviceStatistic::current()->entryName) {
				UARTprintf("%s=%s\n", deviceStatistic::current()->entryName, deviceStatistic::current()->getValue().c_str());
			}
			taskEXIT_CRITICAL(); //this is only for the systematic use of print guarding

			taskEXIT_CRITICAL();
		}
	}

	netbuf_delete(reply);
	_requestPost = !_requestPost;
	return true;
}

netbuf* Exosite::generateRequest() {
	_requestPost ?
			_exositeRequestFactory.writeRequest(_deviceRequestFactory.writeRequest(), _workerBuffer):
			_exositeRequestFactory.readRequest(_deviceRequestFactory.readRequest(), _workerBuffer);

	netbuf* buf = netbuf_new();
	netbuf_ref(buf, _workerBuffer.data(), _workerBuffer.length());
	return buf;
}

// =============================================================================
//! \file
//! \copyright
// ========================= end of file: exosite.cpp ==========================
