#ifndef _OLR_CONTROLLER_LIB_h
#define _OLR_CONTROLLER_LIB_h

#ifdef __cplusplus

extern "C"
{
#endif

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "HardwareConfiguration.h"
#include "GameConfiguration.h"
#include "EnumsAndConstants.h"

    extern int DIGITAL_CTRL[MAX_PLAYER_NUMBER]; // Global Array containig PINs used for the Digital Controllers (ex: Push Buttons)
    // int DIGITAL_CTRL[MAX_PLAYER_NUMBER]; // Global Array containig PINs used for the Digital Controllers (ex: Push Buttons)

    void controller_setup(void);

    void controller_init(OneController *ct, enum ControllerType mode, int pin);

    byte controller_getStatus(OneController *ct);

    float controller_getSpeed(OneController *ct);

    float controller_getAccel(void);

    bool controller_isActive(int pin);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
