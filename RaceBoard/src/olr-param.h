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
#include "GameConfiguration.h"

    //////////////////////////////////////////////////////////////////

    void param_setdefault(struct ConfigurationParameter *cfg, uint16_t numberOfLeds);
    void param_option_set(struct ConfigurationParameter *cfg, uint8_t option, boolean value);
    boolean param_option_is_active(struct ConfigurationParameter *cfg, uint8_t option);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
