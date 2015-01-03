// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			29.12.2014.
//! \note
// =============================================================================
#ifndef _WEATHERREPORT_H_
#define _WEATHERREPORT_H_

#include "../projectconfiguration.hpp"

namespace service {
namespace weatherTask {

class report {
    //
    // The brief weather description, this is caller provided.
    //
	public: const s8* Description;

    //
    // The current temperature(units determined by caller).
    //
    public: s32 Temp;

    //
    // The daily high temperature(units determined by caller).
    //
    public: s32 TempHigh;

    //
    // The daily low temperature(units determined by caller).
    //
    public: s32 TempLow;

    //
    // The current humidity(units determined by caller).
    //
    public: s32 Humidity;

    //
    // The current atmospheric pressure(units determined by caller).
    //
    public: s32 Pressure;

    //
    // The last time these values were update (GMT Unix time).
    //
    public: s32 Time;

    //
    // The sunrise time (GMT Unix time).
    //
    public: u32 SunRise;

    //
    // The sunset time (GMT Unix time).
    //
    public: u32 SunSet;

    //
    // Icon image.
    //
    const u8 *pui8Image;
};

} // weatherTask

} // manager

#endif // _WEATHERREPORT_H_
// =============================================================================
//! \file
//! \copyright
// ==================== end of file: weatherreport.hpp =========================
