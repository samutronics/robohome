// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			30.12.2014.
//! \note
// =============================================================================
#include "exosite.hpp"
#include "mediator.hpp"
#include "devicestatistic.hpp"
#include "commandsiterator.hpp"
#include "exositerequestfactory.hpp"
#include "projectconfiguration.hpp"

using namespace std;
using namespace libs;
using namespace service::exosite;

bool Exosite::processingReply(netbuf* reply) {
	if(_requestPost) {
		_exositeRequestFactory.parseWriteResult(reply->p);
	}
	else {
		_exositeRequestFactory.parseReadResult(reply->p, _workerBuffer);

		string tmp;
		CommandsIterator it(_workerBuffer);
		it.next();
		MediatorFactory::get()->execute(it, tmp);
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
