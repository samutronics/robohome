//*****************************************************************************
//
// eth_client.c - This file handles all of the Ethernet connections using lwIP.
//
// Copyright (c) 2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.0.12573 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>

#include "hw_memmap.h"
#include "hw_types.h"
#include "flash.h"
#include "gpio.h"
#include "interrupt.h"
#include "sysctl.h"
#include "lwiplib.h"
#include "systick.h"
#include "lwip/opt.h"
#include "lwip/dns.h"
#include "eth_client.h"
#include "json.h"

//*****************************************************************************
//
// g_sEnet.ulRequest Values
//
//*****************************************************************************
#define WEATHER_NONE            0
#define WEATHER_CURRENT         1
#define WEATHER_FORECAST        2

//*****************************************************************************
//
// The current state of the Ethernet connection.
//
//*****************************************************************************
struct
{
    //
    // Global define of the TCP structure used.
    //
    struct tcp_pcb *psTCP;

    //
    // Global IP structure to hold a copy of the DNS resolved address.
    //
    struct ip_addr sServerIP;

    //
    // The saved proxy name as a text string.
    //
    const char *pcProxyName;

    volatile enum
    {
        iEthNoConnection,
        iEthDHCPWait,
        iEthDHCPComplete,
        iEthDNSWait,
        iEthTCPConnectWait,
        iEthTCPConnectComplete,
        iEthQueryWait,
        iEthTCPOpen,
        iEthIdle
    } eState;

    unsigned long ulRequest;
}
g_sEnet;

//*****************************************************************************
//
// Maximum size of an weather request.
//
//*****************************************************************************
#define MAX_REQUEST             256

//*****************************************************************************
//
// Various strings used to access weather information on the web.
//
//*****************************************************************************
static const char g_cWeatherRequest[] = "GET http://api.openweathermap.org/data/2.5/weather?q=";
static const char g_cWeatherRequestForecast[] = "GET http://api.openweathermap.org/data/2.5/forecast/daily?q=";
static const char g_cMode[] = "&mode=json&units=metric";
static const char g_cAPPIDOpenWeather[] = "&APIID=afc5370fef1dfec1666a5676346b163b";
static const char g_cHTTP11[] = " HTTP/1.0\r\n\r\n";

//*****************************************************************************
//
// This structure holds the state and control values for the weather requests.
//
//*****************************************************************************
struct
{
    //
    // The current weather source.
    //
    tWeatherSource eWeatherSource;

    //
    // The format expected from the weather source.
    //
    enum
    {
        iFormatJSON
    }
    eFormat;

    //
    // The application provided callback function.
    //
    tEventFunction pfnEvent;

    //
    // The application provided weather information structure.
    //
    tWeatherReport *psWeatherReport;

    //
    // The local buffer used to store the current weather request.
    //
    char pcRequest[MAX_REQUEST];

    //
    // The number of valid bytes in the request.
    //
    uint32_t ui32RequestSize;
}
g_sWeather;

static void DNSServerFound(const char *pcName, struct ip_addr *psIPAddr, void *vpArg);

//*****************************************************************************
//
// Close an active connection.
//
//*****************************************************************************
void
EthClientReset(void)
{
    //
    // No longer have a link.
    //
    g_sEnet.eState = iEthNoConnection;

    //
    // Reset the addresses.
    //
    g_sEnet.sServerIP.addr = 0;

    //
    // Deallocate the TCP structure if it was already allocated.
    //
    if(g_sEnet.psTCP)
    {
        //
        // Clear out all of the TCP callbacks.
        //
        tcp_sent(g_sEnet.psTCP, NULL);
        tcp_recv(g_sEnet.psTCP, NULL);
        tcp_err(g_sEnet.psTCP, NULL);

        //
        // Close the TCP connection.
        //
        tcp_close(g_sEnet.psTCP);
        g_sEnet.psTCP = 0;
    }
}

//*****************************************************************************
//
// Handles lwIP TCP/IP errors.
//
// \param vPArg is the state data for this connection.
// \param iErr is the error that was detected.
//
// This function is called when the lwIP TCP/IP stack has detected an error.
// The connection is no longer valid.
//
// \return None.
//
//*****************************************************************************
static void
TCPError(void *vPArg, err_t iErr)
{
}

//*****************************************************************************
//
// Finalizes the TCP connection in client mode.
//
// \param pvArg is the state data for this connection.
// \param psPcb is the pointer to the TCP control structure.
// \param psBuf is the buffer structure that holds the data for this receive
// event.
// \param iErr is not used in this implementation.
//
// This function is called when the lwIP TCP/IP stack has completed a TCP
// connection.
//
// \return This function will return an lwIP defined error code.
//
//*****************************************************************************
static err_t
TCPReceive(void *pvArg, struct tcp_pcb *psPcb, struct pbuf *psBuf, err_t iErr)
{
    struct pbuf *psBufCur;
    int32_t i32Items;

    if(psBuf == 0)
    {
        //
        // Tell the application that the connection was closed.
        //
        if(g_sWeather.pfnEvent)
        {
            g_sWeather.pfnEvent(ETH_EVENT_CLOSE, 0, 0);
            g_sWeather.pfnEvent = 0;
        }

        //
        // Close out the port.
        //
        tcp_close(psPcb);

        if(psPcb == g_sEnet.psTCP)
        {
            g_sEnet.psTCP = 0;
        }

        g_sEnet.eState = iEthIdle;

        return(ERR_OK);
    }

    if(g_sEnet.eState == iEthQueryWait)
    {
        if(g_sEnet.ulRequest == WEATHER_CURRENT)
        {
            //
            // Read items from the buffer.
            //
            i32Items = JSONParseCurrent(0, g_sWeather.psWeatherReport, psBuf);

            //
            // Make sure some items were found.
            //
            if(i32Items > 0)
            {
                if(g_sWeather.pfnEvent)
                {
                    g_sWeather.pfnEvent(ETH_EVENT_RECEIVE,
                                        (void *)g_sWeather.psWeatherReport, 0);

                    //
                    // Clear the event function and return to the idle state.
                    //
                    g_sEnet.eState = iEthIdle;
                }
            }
            else if(i32Items < 0)
            {
                if(g_sWeather.pfnEvent)
                {
                    //
                    // This was not a valid request.
                    //
                    g_sWeather.pfnEvent(ETH_EVENT_INVALID_REQ, 0, 0);

                    //
                    // Clear the event function and return to the idle state.
                    //
                    g_sEnet.eState = iEthIdle;
                }
            }
        }
        else if(g_sEnet.ulRequest == WEATHER_FORECAST)
        {
            //
            // Read items from the buffer.
            //
            i32Items = JSONParseForecast(0, g_sWeather.psWeatherReport, psBuf);

            if(i32Items > 0)
            {
                if(g_sWeather.pfnEvent)
                {
                    g_sWeather.pfnEvent(ETH_EVENT_RECEIVE,
                                        (void *)g_sWeather.psWeatherReport, 0);

                    //
                    // Clear the event function and return to the idle state.
                    //
                    g_sEnet.eState = iEthIdle;
                }
            }
            else if(i32Items < 0)
            {
                if(g_sWeather.pfnEvent)
                {
                    //
                    // This was not a valid request.
                    //
                    g_sWeather.pfnEvent(ETH_EVENT_INVALID_REQ, 0, 0);

                    //
                    // Clear the event function and return to the idle state.
                    //
                    g_sEnet.eState = iEthIdle;
                }
            }
        }
    }
    else
    {
        //
        // Go to idle state.
        //
        g_sEnet.eState = iEthIdle;
    }

    //
    // Initialize the linked list pointer to parse.
    //
    psBufCur = psBuf;

    //
    // Free the buffers used since they have been processed.
    //
    while(psBufCur->len != 0)
    {
        //
        // Indicate that you have received and processed this set of TCP data.
        //
        tcp_recved(psPcb, psBufCur->len);

        //
        // Go to the next buffer.
        //
        psBufCur = psBufCur->next;

        //
        // Terminate if there are no more buffers.
        //
        if(psBufCur == 0)
        {
            break;
        }
    }

    //
    // Free the memory space allocated for this receive.
    //
    pbuf_free(psBuf);

    //
    // Return.
    //
    return(ERR_OK);
}

//*****************************************************************************
//
// Handles acknowledgment of data transmitted via Ethernet.
//
// \param pvArg is the state data for this connection.
// \param psPcb is the pointer to the TCP control structure.
// \param ui16Len is the length of the data transmitted.
//
// This function is called when the lwIP TCP/IP stack has received an
// acknowledgment for data that has been transmitted.
//
// \return This function will return an lwIP defined error code.
//
//*****************************************************************************
static err_t
TCPSent(void *pvArg, struct tcp_pcb *psPcb, u16_t ui16Len)
{
    //
    // Return OK.
    //
    return (ERR_OK);
}

//*****************************************************************************
//
// Finalizes the TCP connection in client mode.
//
// \param pvArg is the state data for this connection.
// \param psPcb is the pointer to the TCP control structure.
// \param iErr is not used in this implementation.
//
// This function is called when the lwIP TCP/IP stack has completed a TCP
// connection.
//
// \return This function will return an lwIP defined error code.
//
//*****************************************************************************
static err_t
TCPConnected(void *pvArg, struct tcp_pcb *psPcb, err_t iErr)
{
    //
    // Check if there was a TCP error.
    //
    if(iErr != ERR_OK)
    {
        //
        // Clear out all of the TCP callbacks.
        //
        tcp_sent(psPcb, NULL);
        tcp_recv(psPcb, NULL);
        tcp_err(psPcb, NULL);

        //
        // Close the TCP connection.
        //
        tcp_close(psPcb);

        if(psPcb == g_sEnet.psTCP)
        {
            g_sEnet.psTCP = 0;
        }

        //
        // And return.
        //
        return (ERR_OK);
    }

    //
    // Setup the TCP receive function.
    //
    tcp_recv(psPcb, TCPReceive);

    //
    // Setup the TCP error function.
    //
    tcp_err(psPcb, TCPError);

    //
    // Setup the TCP sent callback function.
    //
    tcp_sent(psPcb, TCPSent);

    //
    // Connection is complete.
    //
    g_sEnet.eState = iEthTCPConnectComplete;

    //
    // Return a success code.
    //
    return(ERR_OK);
}

//*****************************************************************************
//
// TCP connect
//
// This function attempts to connect to a TCP endpoint.
//
// \return None.
//
//*****************************************************************************
err_t
EthClientTCPConnect(uint32_t ui32Port)
{
    err_t eTCPReturnCode;

    if(g_sEnet.psTCP)
    {
        //
        // Initially clear out all of the TCP callbacks.
        //
        tcp_sent(g_sEnet.psTCP, NULL);
        tcp_recv(g_sEnet.psTCP, NULL);
        tcp_err(g_sEnet.psTCP, NULL);

        //
        // Make sure there is no lingering TCP connection.
        //
        tcp_close(g_sEnet.psTCP);
    }

    if(0 == g_sEnet.sServerIP.addr || 0xFFFFFFFF == g_sEnet.sServerIP.addr) {
        dns_gethostbyname("api.openweathermap.org", &g_sEnet.sServerIP, DNSServerFound, 0);
    	while(0 == g_sEnet.sServerIP.addr || 0xFFFFFFFF == g_sEnet.sServerIP.addr);
    }

    //
    // Create a new TCP socket.
    //
    g_sEnet.psTCP = tcp_new();

    //
    // Check if you need to go through a proxy.
    //
    if(g_sEnet.pcProxyName != 0)
    {
        //
        // Attempt to connect through the proxy server.
        //
        eTCPReturnCode = tcp_connect(g_sEnet.psTCP, &g_sEnet.sServerIP,
                                     ui32Port, TCPConnected);
    }
    else
    {
        //
        // Attempt to connect to the server directly.
        //
        eTCPReturnCode = tcp_connect(g_sEnet.psTCP, &g_sEnet.sServerIP,
                                     ui32Port, TCPConnected);
    }

    return(eTCPReturnCode);
}

//*****************************************************************************
//
// TCP connect
//
// This function attempts to connect to a TCP endpoint.
//
// \return None.
//
//*****************************************************************************
void
EthClientTCPDisconnect(void)
{
    //
    // No longer have a link.
    //
    g_sEnet.eState = iEthNoConnection;

    //
    // Deallocate the TCP structure if it was already allocated.
    //
    if(g_sEnet.psTCP)
    {
        //
        // Close the TCP connection.
        //
        tcp_close(g_sEnet.psTCP);
        g_sEnet.psTCP = 0;
    }
}

//*****************************************************************************
//
// Handler function when the DNS server gets a response or times out.
//
// \param pcName is DNS server name.
// \param psIPAddr is the DNS server's IP address.
// \param vpArg is the configurable argument.
//
// This function is called when the DNS server resolves an IP or times out.
// If the DNS server returns an IP structure that is not NULL, add the IP to
// to the g_sEnet.sServerIP IP structure.
//
// \return None.
//
//*****************************************************************************
static void
DNSServerFound(const char *pcName, struct ip_addr *psIPAddr, void *vpArg)
{
    //
    // Check if a valid DNS server address was found.
    //
    if(psIPAddr != NULL)
    {
        //
        // Copy the returned IP address into a global IP address.
        //
        g_sEnet.sServerIP = *psIPAddr;
    }
    else
    {
    }
}

//*****************************************************************************
//
// Required by lwIP library to support any host-related timer functions.
//
//*****************************************************************************
void
lwIPHostTimerHandler(void)
{
}

//*****************************************************************************
//
// Send a request to the server
//
// \param pcRequest request to be sent
// \param ui32Size length of the request to be sent. this is usually the size
// of the request minus the termination character
//
// This function will send the request to the connected server
//
// \return the lwIP error code.
//
//*****************************************************************************
err_t
EthClientSend(char *pcRequest, uint32_t ui32Size)
{
    err_t eError;

    eError = tcp_write(g_sEnet.psTCP, pcRequest, ui32Size, TCP_WRITE_FLAG_COPY);

    //
    //  Write data for sending (but does not send it immediately).
    //
    if(eError == ERR_OK)
    {
        //
        // Find out what we can send and send it
        //
        tcp_output(g_sEnet.psTCP);
    }

    return(eError);
}

//*****************************************************************************
//
// Returns the weather server IP address for this interface.
//
// This function will read and return the weather server IP address that is
// currently in use.  This could be the proxy server if the Internet proxy
// is enabled.
//
// \return Returns the weather server IP address for this interface.
//
//*****************************************************************************
uint32_t
EthClientServerAddrGet(void)
{
    //
    // Return IP.
    //
    return((uint32_t)g_sEnet.sServerIP.addr);
}

//*****************************************************************************
//
// Set the proxy string for the Ethernet connection.
//
// \param pcProxyName is the string used as the proxy server name.
//
// This function sets the current proxy used by the Ethernet connection.  The
// \e pcProxyName value can be 0 to indicate that no proxy is in use or it can
// be a pointer to a string that holds the name of the proxy server to use.
// The content of the pointer passed to \e pcProxyName should not be changed
// after this call as this function only stores the pointer and does not copy
// the data from this pointer.
//
// \return None.
//
//*****************************************************************************
void
EthClientProxySet(const char *pcProxyName)
{
    //
    // Save the new proxy string.
    //
    g_sEnet.pcProxyName = pcProxyName;

    //
    // Reset the connection on any change to the proxy.
    //
    EthClientReset();
}

//*****************************************************************************
//
// Set the current weather source.
//
//*****************************************************************************
void
WeatherSourceSet(tWeatherSource eWeatherSource)
{
    //
    // Save the source.
    //
    g_sWeather.eWeatherSource = eWeatherSource;
    g_sWeather.eFormat = iFormatJSON;
}

//*****************************************************************************
//
// Merges strings into the current request at a given pointer and offset.
//
//*****************************************************************************
static int32_t
MergeRequest(int32_t i32Offset, const char *pcSrc, int32_t i32Size,
             bool bReplaceSpace)
{
    int32_t i32Idx;

    //
    // Copy the base request to the buffer.
    //
    for(i32Idx = 0; i32Idx < i32Size; i32Idx++)
    {
        if((pcSrc[i32Idx] == ' ') && (bReplaceSpace))
        {
            if((i32Offset + 3) >= sizeof(g_sWeather.pcRequest))
            {
                break;
            }
            g_sWeather.pcRequest[i32Offset++] = '%';
            g_sWeather.pcRequest[i32Offset++] = '2';
            g_sWeather.pcRequest[i32Offset] = '0';
        }
        else
        {
            g_sWeather.pcRequest[i32Offset] = pcSrc[i32Idx];
        }

        if((i32Offset >= sizeof(g_sWeather.pcRequest)) ||
           (pcSrc[i32Idx] == 0))
        {
            break;
        }

        i32Offset++;
    }

    return(i32Offset);
}

//*****************************************************************************
//
// Gets the daily forecast for a given city.
//
//*****************************************************************************
int32_t
WeatherForecast(tWeatherSource eWeatherSource, const char *pcQuery,
                tWeatherReport *psWeatherReport, tEventFunction pfnEvent)
{
    int32_t i32Idx;
    const char pcCount[] = "&cnt=1";

    //
    // If the requested source is not valid or there is no call back then
    // just fail.
    //
    if((eWeatherSource != iWSrcOpenWeatherMap) || (g_sWeather.pfnEvent))
    {
        return (-1);
    }

    g_sWeather.pfnEvent = pfnEvent;
    g_sWeather.psWeatherReport = psWeatherReport;

    //
    // Connect or reconnect to port 80.
    //
    g_sEnet.eState = iEthTCPConnectWait;

    //
    // Copy the base forecast request to the buffer.
    //
    i32Idx = MergeRequest(0, g_cWeatherRequestForecast,
                          sizeof(g_cWeatherRequestForecast), false);

    //
    // Append the request.
    //
    i32Idx = MergeRequest(i32Idx, pcQuery, sizeof(g_sWeather.pcRequest),
                          true);

    //
    // Append the request mode.
    //
    i32Idx = MergeRequest(i32Idx, g_cMode, sizeof(g_cMode), false);

    //
    // Append the count.
    //
    i32Idx = MergeRequest(i32Idx, pcCount, sizeof(pcCount), false);

    //
    // Append the App ID.
    //
    i32Idx = MergeRequest(i32Idx, g_cAPPIDOpenWeather,
                          sizeof(g_cAPPIDOpenWeather), false);

    //
    // Append the "HTTP:/1.1" string.
    //
    i32Idx = MergeRequest(i32Idx, g_cHTTP11, sizeof(g_cHTTP11), false);

    //
    // Forcast weather report request.
    //
    g_sEnet.ulRequest = WEATHER_FORECAST;

    if(EthClientTCPConnect(80) != ERR_OK)
    {
        return(-1);
    }

    //
    // Save the size of the request.
    //
    g_sWeather.ui32RequestSize = i32Idx;

    return(0);
}

//*****************************************************************************
//
// Gets the current weather information for a given city.
//
//*****************************************************************************
int32_t
WeatherCurrent(tWeatherSource eWeatherSource, const char *pcQuery,
               tWeatherReport *psWeatherReport, tEventFunction pfnEvent)
{
    int32_t i32Idx;

    //
    // If the requested source is not valid or there is no call back then
    // just fail.
    //
    if((eWeatherSource != iWSrcOpenWeatherMap) || (g_sWeather.pfnEvent))
    {
        return (-1);
    }

    g_sWeather.pfnEvent = pfnEvent;
    g_sWeather.psWeatherReport = psWeatherReport;

    //
    // Copy the base current request to the buffer.
    //
    i32Idx = MergeRequest(0, g_cWeatherRequest, sizeof(g_cWeatherRequest),
                          false);

    //
    // Append the request.
    //
    i32Idx = MergeRequest(i32Idx, pcQuery, sizeof(g_sWeather.pcRequest), true);

    //
    // Append the request mode.
    //
    i32Idx = MergeRequest(i32Idx, g_cMode, sizeof(g_cMode), false);

    //
    // Append the App ID.
    //
    i32Idx = MergeRequest(i32Idx, g_cAPPIDOpenWeather,
                          sizeof(g_cAPPIDOpenWeather), false);

    //
    // Append the "HTTP:/1.1" string.
    //
    i32Idx = MergeRequest(i32Idx, g_cHTTP11, sizeof(g_cHTTP11), false);

    //
    // Save the size of this request.
    //
    g_sWeather.ui32RequestSize = i32Idx;

    //
    // Connect or reconnect to port 80.
    //
    g_sEnet.eState = iEthTCPConnectWait;

    //
    // Current weather report request.
    //
    g_sEnet.ulRequest = WEATHER_CURRENT;

    //
    // Connect to server
    //
    if(EthClientTCPConnect(80) != ERR_OK)
    {
        return(-1);
    }

    while(g_sEnet.psTCP->state != ESTABLISHED);

    EthClientSend(g_sWeather.pcRequest, g_sWeather.ui32RequestSize);

    return(0);
}

