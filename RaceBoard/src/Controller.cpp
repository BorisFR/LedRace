#include "Controller.h"

Controller::Controller() {}

void Controller::Setup()
{
    DIGITAL_CTRL[CTRL_1] = DIG_CTRL_1_PIN;
    DIGITAL_CTRL[CTRL_2] = DIG_CTRL_2_PIN;
    DIGITAL_CTRL[CTRL_3] = DIG_CTRL_3_PIN;
    DIGITAL_CTRL[CTRL_4] = DIG_CTRL_4_PIN;

    pinMode(DIG_CTRL_1_PIN, INPUT_PULLUP); // pull up in adc
    pinMode(DIG_CTRL_2_PIN, INPUT_PULLUP);
    pinMode(DIG_CTRL_3_PIN, INPUT_PULLUP);
    pinMode(DIG_CTRL_4_PIN, INPUT_PULLUP);
}

void Controller::controller_init(OneController *controller, ControllerType mode, byte carNumber)
{
    controller->mode = mode;
    controller->pin = DIGITAL_CTRL[carNumber];
    controller->delta_analog = DELTA_ANALOG;
}

byte Controller::controller_getStatus(OneController *ct)
{

    if (ct->mode == DIGITAL_MODE)
    {
        return digitalRead(ct->pin);
    }
    else if (ct->mode == ANALOG_MODE)
    {
        ct->adc = analogRead(ct->pin);
        if (abs(ct->badc - ct->adc) > ct->delta_analog)
        {
            ct->badc = ct->adc;
            return 1;
        }
        ct->badc = ct->adc;
    }
    else if (ct->mode == DEMO_MODE)
    {
        ct->adc++;
        int rnd = rand();
        long threshold = 1800 - rnd;
        if (ct->adc >= (threshold))
        {
            ct->adc = 0;
            return 1;
        }
    }
    return 0;
}

float Controller::controller_getSpeed(OneController *ct)
{
    float speed = 0.0;
    if ((ct->flag_sw == 1) && (controller_getStatus(ct) == 0))
    {
        ct->flag_sw = 0;
        speed = CAR_ACCELERATION;
    }

    if ((ct->flag_sw == 0) && (controller_getStatus(ct) == 1))
    {
        ct->flag_sw = 1;
    }
    return speed;
}

float Controller::controller_getAccel(void)
{
    return CAR_ACCELERATION;
}

bool Controller::controller_isActive(byte carNumber)
{
    return !digitalRead(DIGITAL_CTRL[carNumber]);
}

/*void Controller::set_controllers_mode(OneController *controller, ControllerType mode)
{
    for (uint8_t i = 0; i < MAX_PLAYER_NUMBER; ++i)
    {
        controller_init(controller, mode, i);
    }
}*/