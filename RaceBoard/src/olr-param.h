#ifndef _OLR_PARAM_LIB_h
#define _OLR_PARAM_LIB_h

#ifdef __cplusplus

extern "C"
{
#endif

#include "Arduino.h"
#include <stdint.h>
#include <stdbool.h>
#include "EnumsAndConstants.h"

// Default values loaded on "D" command received (Serial Protocol)
//////////////////////////////////////////////////////////////////
#define BOXLEN 10
#define NUMLAP 5
#define BATTERY_MODE false
#define AUTOSTART_MODE false
#define BOX_ALWAYS_ON false
#define SLOPE_ALWAYS_ON true
#define PLAYER_3 false
#define PLAYER_4 false
#define DEMO_MODE_ST false
    //////////////////////////////////////////////////////////////////

    void param_setdefault(struct ConfigurationParameter *cfg, uint16_t numberOfLeds);
    void param_option_set(struct ConfigurationParameter *cfg, uint8_t option, boolean value);
    boolean param_option_is_active(struct ConfigurationParameter *cfg, uint8_t option);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
