#include "Controller.h"

Controller::Controller() {}

void Controller::Setup()
{
    DIGITAL_CTRL[CTRL_1] = DIG_CTRL_1_PIN;
    DIGITAL_CTRL[CTRL_2] = DIG_CTRL_2_PIN;
    DIGITAL_CTRL[CTRL_3] = DIG_CTRL_3_PIN;
    DIGITAL_CTRL[CTRL_4] = DIG_CTRL_4_PIN;
    CtrlType[CTRL_1] = PIN_TYPE_PLAYER_1;
    CtrlType[CTRL_2] = PIN_TYPE_PLAYER_2;
    CtrlType[CTRL_3] = PIN_TYPE_PLAYER_3;
    CtrlType[CTRL_4] = PIN_TYPE_PLAYER_4;
    pinMode(DIG_CTRL_1_PIN, PIN_TYPE_PLAYER_1); // pull up in adc
    pinMode(DIG_CTRL_2_PIN, PIN_TYPE_PLAYER_2);
    pinMode(DIG_CTRL_3_PIN, PIN_TYPE_PLAYER_3);
    pinMode(DIG_CTRL_4_PIN, PIN_TYPE_PLAYER_4);
}

void Controller::controller_init(OneController *controller, ControllerType mode, byte carNumber)
{
    controller->mode = mode;
    controller->pin = DIGITAL_CTRL[carNumber];
    controller->delta_analog = DELTA_ANALOG;
    controller->number = carNumber;
}

byte Controller::controller_getStatus(OneController *ct)
{

    if (ct->mode == DIGITAL_MODE)
    {
        // return digitalRead(ct->pin);
        if (CtrlType[ct->number] == INPUT_PULLUP)
        {
            return !digitalRead(ct->pin);
        }
        else
        {
            return digitalRead(ct->pin);
        }
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
    if (CtrlType[carNumber] == INPUT_PULLUP)
    {
        return !digitalRead(DIGITAL_CTRL[carNumber]);
    }
    else
    {
        return digitalRead(DIGITAL_CTRL[carNumber]);
    }
}

/*void Controller::set_controllers_mode(OneController *controller, ControllerType mode)
{
    for (uint8_t i = 0; i < MAX_PLAYER_NUMBER; ++i)
    {
        controller_init(controller, mode, i);
    }
}*/