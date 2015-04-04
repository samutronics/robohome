// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#include "ff.h"
#include "web.hpp"
#include "mediator.hpp"
#include "projectmanager.hpp"
#include "commandsiterator.hpp"

const sp8 httpMethods[] = {
		"GET ",
		"POST ",
		"HEAD ",
		""
};

cs8 httpHeader[] =
		"HTTP/1.1 200 OK\r\n\
		Connection: close\r\n\
		Content-Length: ";

using namespace std;
using namespace libs;
using namespace systemGlobal;
using namespace service::web;
using namespace manager::project;
using namespace service::web::configuration;

Web::Web(): _connectionFromClient(NULL), _httpRequest(512, 0), _operationResult(512, 0) {
	// =============================================================================
	//! * Enable GPIO Port F, and configure Pin 0 and Pin 4 for Ethernet LEDs
	// =============================================================================
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinConfigure(GPIO_PF0_EN0LED0);
	GPIOPinConfigure(GPIO_PF4_EN0LED1);
	GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_DIR_MODE_HW);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

	// =============================================================================
	//! * Read the MAC address from user registers. If there is no valid MAC,
	//!		the thread stays in infinite loop. In case of valid MAC address, it
	//!		will be formatted into 8x1 byte format.
	// =============================================================================
	u32 usr0;
	u32 usr1;
    FlashUserGet(&usr0, &usr1);
    if((usr0 == 0xffffffff) || (usr1 == 0xffffffff)) {while(1);}

    u8 mac[8];
    mac[0] = ((usr0 >>  0) & 0xff);
    mac[1] = ((usr0 >>  8) & 0xff);
    mac[2] = ((usr0 >> 16) & 0xff);
    mac[3] = ((usr1 >>  0) & 0xff);
    mac[4] = ((usr1 >>  8) & 0xff);
    mac[5] = ((usr1 >> 16) & 0xff);

    u32 IP = 0;
    u32 NetMask = 0;
    u32 GateWay = 0;

    // =============================================================================
    //! * Initialize the LwIP stack and block the thread until a valid
    //!		IP adress will be there.
    // =============================================================================
    ProjectManagerFactory::get()->sysConfig().network(static_cast<u32&>(IP), static_cast<u32&>(NetMask), static_cast<u32&>(GateWay));
    lwIPInit(currentSystemClockFrequency, mac, IP, NetMask, GateWay, (0 == IP) ? IPADDR_USE_DHCP : IPADDR_USE_STATIC);
    while((0 == IP) && ((0xFFFFFFFF == lwIPLocalIPAddrGet()) || (0x0 == lwIPLocalIPAddrGet()))) {taskYIELD();}

	// =============================================================================
	//! * Print the gathered IP address to the terminal. Guard is required, to
    //!		prevent the message fregmentation. Anonymous block is for spare some
    //!		stack space.
    // =============================================================================
    {
    	taskENTER_CRITICAL();
    	UARTprintf("Device MAC address: %x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    	u32 IP = lwIPLocalIPAddrGet();
    	UARTprintf("Device IP address: %d.%d.%d.%d\n", (IP & 0xff), ((IP >> 8) & 0xff), ((IP >> 16) & 0xff), ((IP >> 24) & 0xff));
    	taskEXIT_CRITICAL();
    }
}

void Web::task(void *pvParameters) {
	while(true) {
		netconn* connection = netconn_new (NETCONN_TCP);
		netconn_bind(connection, IPADDR_ANY, port);
		netconn_listen(connection);
		while(true)
		{
			// wait for an incomming connection from the remote client
			_connectionFromClient = NULL;
			if (ERR_OK != netconn_accept(connection, &_connectionFromClient)) {
				UARTprintf("An error occured during connection accept\n");
				break;
			}

			// get data from the connection request
			netbuf* reply = NULL;
			if (ERR_OK != netconn_recv(_connectionFromClient, &reply)) {
				UARTprintf("Failed to receive data\n");
				netconn_close(_connectionFromClient);
				netconn_delete(_connectionFromClient);
				break;
			}

			// put the data into an std::string object to the parse becames more easier
			_httpRequest.clear();
			_httpRequest.append(static_cast<s8*>(reply->p->payload), reply->p->len);
			// the std::string object took over the data. Delete the netbuf to avoid the memory leaks
			netbuf_delete(reply);

			// serve the request by the method type
			switch(getHTTPMethodType(_httpRequest)) {
			case get: {
				u32 startOfURI = _httpRequest.find("/");
				if(string::npos == startOfURI) {
					UARTprintf("%s\n", _httpRequest.c_str());
					break;
				}

				u32 startOfArguments = _httpRequest.find(argsPattern);
				if(string::npos != startOfArguments) {
					parseArgs(startOfArguments + sizeof(argsPattern) - 1);
					break;
				}

				parseResource(startOfURI);
				break;
			}
			default: {
				UARTprintf("unsupported method\n");
				while(true);
			}
			}

			// freeing the
			netconn_close(_connectionFromClient);
			netconn_delete(_connectionFromClient);
		}

		netconn_close(connection);
		netconn_delete(connection);
	}
}

bool Web::parseArgs(cu32 startOfArguments) {
	u32 endOfArguments = _httpRequest.find(' ', startOfArguments);
	if(string::npos == endOfArguments) {return false;}

	CommandsIterator it(_httpRequest.substr(startOfArguments, endOfArguments - startOfArguments));
	while(it.next()) {
		_operationResult.clear();
		MediatorFactory::get()->execute(it, _operationResult);
		string header;
		makeHttpHeader(header, _operationResult.size());
		if (ERR_OK != netconn_write(_connectionFromClient, header.c_str(), header.size(), NETCONN_COPY)) {UARTprintf("Failed to send default page\n");}
		if (ERR_OK != netconn_write(_connectionFromClient, _operationResult.c_str(), _operationResult.size(), NETCONN_NOCOPY)) {UARTprintf("Failed to send default page\n");}
	}

	return true;
}

bool Web::parseResource(cu32 startOfURI) const {
	string uri;
	if('/' == _httpRequest[startOfURI] && ' ' == _httpRequest[startOfURI + 1]) {
		uri = defaultPage;
	}
	else {
		u32 endOfURI = _httpRequest.find(" ", startOfURI);
		uri = _httpRequest.substr(startOfURI, endOfURI - startOfURI);
	}

	u32 length = 0;
	u8* resource = NULL;
	if(FR_OK != readResource(uri.c_str(), resource, length)) {UARTprintf("Failed to get resource\n");}

	string header;
	makeHttpHeader(header, length);
	if (ERR_OK != netconn_write(_connectionFromClient, header.c_str(), header.size(), NETCONN_COPY)) {UARTprintf("Failed to send default page\n");}
	if (ERR_OK != netconn_write(_connectionFromClient, resource, length, NETCONN_NOCOPY)) {UARTprintf("Failed to send default page\n");}

	delete resource;
	return true;
}

Web::httpMethod Web::getHTTPMethodType(const std::string& request) const {
	for(u32 method = get; method <= last_httpVersion_element; method++) {
		if(0 == request.find(httpMethods[method])) {return static_cast<httpMethod>(method);}
	}

	return last_httpMethod_element;
}

void Web::makeHttpHeader(std::string& header, cu32 lenght) const {
	header = httpHeader;
	s8 buf[10];
	sprintf(buf, "%d\r\n\r\n", lenght);
	header += buf;
}

// =============================================================================
//! \file
//! \copyright
// =========================== end of file: web.cpp ============================
