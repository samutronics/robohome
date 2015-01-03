//*****************************************************************************
//
// json.h - The parsing function headers for JSON data from the web server.
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

#ifndef __JSON_H__
#define __JSON_H__

#include <stdint.h>

#include "weatherreport.hpp"

#define INVALID_INT             ((int32_t)(0x80000000))

namespace manager {
namespace weatherTask {

int32_t JSONParseCurrent(uint32_t ui32Index, report& psWeatherReport, struct pbuf *psBuf);
int32_t JSONParseForecast(uint32_t ui32Index, report& psWeatherReport, struct pbuf *psBuf);

} // weatherTask

} // manager

#endif