// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			03.12.2014.
//! \note
// =============================================================================
#include "web.hpp"
#include "httpd.h"

const u8 page[] = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\
<head>\
    <title></title>\
    <script language=\"javascript\" type=\"text/javascript\">\
        var timer = setInterval(function () { updateState() }, 5000);\
        function updateState() {\
            var req = createXmlHttpRequestObject();\
            function updateStateReply() {\
\
            }\
\
            if (req) {\
                req.open(\"GET\", \"192.168.1.122/processing?\", true);\
                req.onreadystatechange = updateStateReply;\
                req.send(null);\
            }\
        }\
        function LED1_onclick() {\
            var req = new XmlHttpRequest();\
            req.open(\"GET\", \"request.htm\", false);\
            req.send();\
        }\
\
    </script>\
</head>\
<body>\
    <p style=\"font-weight: 700; text-decoration: underline; text-align: center; font-size: xx-large\">\
        Samutronics home automation testportal</p>\
    <p style=\"font-weight: 700; text-decoration: underline; text-align: center; font-size: xx-large\">\
        <input id=\"LED0\" name=\"R1\" type=\"radio\" value=\"LED0\" /></p>\
    <p style=\"font-weight: 700; text-decoration: underline; text-align: center; font-size: xx-large\">\
        <input id=\"LED1\" name=\"R2\" type=\"radio\" value=\"LED1\" onclick=\"return LED1_onclick()\" /></p>\
    <p style=\"font-weight: 700; text-decoration: underline; text-align: center; font-size: xx-large\">\
        <textarea id=\"RTC\" cols=\"20\" name=\"S1\" rows=\"2\" title=\"RTC of Samutronics\"></textarea></p>\
</body>\
</html>\
";

const sp8 httpMethods[] = {
		"GET ",
		"POST ",
		"HEAD ",
		""
};

using namespace std;
using namespace systemGlobal;
using namespace service::web;
using namespace service::web::configuration;

web::web() {
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

	// =============================================================================
	//! * Initialize the LwIP stack and block the thread until a valid
    //!		IP adress will be there.
	// =============================================================================
    lwIPInit(currentSystemClockFrequency, mac, 0, 0, 0, IPGatheringStrategie);
    while((0xFFFFFFFF == lwIPLocalIPAddrGet()) || (0x0 == lwIPLocalIPAddrGet())) {taskYIELD();}

	// =============================================================================
	//! * Print the gathered IP address to the terminal. Guard is required, to
    //!		prevent the message fregmentation. Anonymous block is for spare some
    //!		stack space.
    // =============================================================================
    {
    	taskENTER_CRITICAL();
    	u32 IP = lwIPLocalIPAddrGet();
    	UARTprintf("Device IP address: %d.%d.%d.%d\n", (IP & 0xff), ((IP >> 8) & 0xff), ((IP >> 16) & 0xff), ((IP >> 24) & 0xff));
    	taskEXIT_CRITICAL();
    }

	// =============================================================================
	//! * Initialize HTTP daemon, that is responsible for the webpage.
    // =============================================================================
}

void web::task(void *pvParameters) {
	while(true) {
		netconn* connection = netconn_new (NETCONN_TCP);
		netconn_bind(connection, IPADDR_ANY, port);
		netconn_listen(connection);
		while(true)
		{
			// wait for an incomming connection from the remote client
			_connectionFromClient = NULL;
			if (ERR_OK != netconn_accept(connection, &_connectionFromClient)) {continue;}

			// get data from the connection request
			netbuf* reply = NULL;
			if (ERR_OK != netconn_recv(_connectionFromClient, &reply)) {
				UARTprintf("Failed to receive data\n");
				netconn_close(_connectionFromClient);
				netconn_delete(_connectionFromClient);
				continue;
			}

			// put the data into an std::string object to the parse becamores easier
			string httpRequest(static_cast<s8*>(reply->p->payload), reply->p->len);
			// the std::string object took over the data. Delete the netbuf to avoid the memory leaks
			netbuf_delete(reply);

			// serve the request by the method type
			switch(getHTTPMethodType(httpRequest)) {
			case get: {
				parseURI(httpRequest);
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
	}
}

bool web::parseURI(const std::string& request) const {
	u32 startOfURI = request.find("/");
	if(string::npos == startOfURI) {return false;}

	if(parseDefaultResource(request, startOfURI)) {return true;}

	u32 startOfArguments = request.find("?");
	if(parseResource(request, startOfURI, startOfArguments)) {return true;}

	return false;
}

bool web::parseResource(const std::string& request, const u32 startOfURI, const u32 startOfArguments) const {
	u32 endOfURI = request.find(" ", startOfURI);
	string URI(request.substr(startOfURI, string::npos == startOfArguments ? endOfURI : startOfArguments));
	if (ERR_OK != netconn_write(_connectionFromClient, (void*)page, sizeof(page), NETCONN_NOCOPY)) {
		UARTprintf("Failed to reply to URI: %s\n", URI.c_str());
		return true;
	}

	return false;
}

bool web::parseDefaultResource(const std::string& request, const u32 startOfURI) const {
	if('/' == request[startOfURI] && ' ' == request[startOfURI + 1]) {
		if (ERR_OK != netconn_write(_connectionFromClient, (void*)page, sizeof(page), NETCONN_NOCOPY)) {
			UARTprintf("Failed to send default page\n");
		}

		return true;
	}

	return false;
}

web::httpMethod web::getHTTPMethodType(const std::string& request) const {
	for(u32 method = get; method <= last_httpVersion_element; method++) {
		if(0 == request.find(httpMethods[method])) {return static_cast<httpMethod>(method);}
	}

	return last_httpMethod_element;
}

// =============================================================================
//! \file
//! \copyright
// =========================== end of file: web.cpp ============================
