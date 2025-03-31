#ifndef CONTROLLER_h
#define CONTROLLER_h

#include <Arduino.h>
#include "HardwareConfiguration.h"
#include "GameConfiguration.h"
#include "EnumsAndConstants.h"

enum
{
    DELTA_ANALOG = 5,
};

class Controller
{
public:
    Controller();
    void Setup();
    void controller_init(OneController *controller, ControllerType mode, byte carNumber);
    byte controller_getStatus(OneController *ct);
    float controller_getSpeed(OneController *ct);
    float controller_getAccel();
    bool controller_isActive(byte carNumber);
    // void set_controllers_mode(OneController *controller, ControllerType mode);

private:
    int DIGITAL_CTRL[MAX_PLAYER_NUMBER];
    uint8_t CtrlType[MAX_PLAYER_NUMBER];

    // static float const Car_ACCELERATION = 0.15;
    //  TODO: 0.2;
};

#endif