#ifndef ENUMS_h
#define ENUMS_h

#include "Arduino.h"

enum DISPLAY_STATES
{
    WELCOME = 0,
    WAITING = 2,
    COUNTDOWN = 4,
    RACE = 6,
    WINNER = 8,
};

typedef struct player_t
{
    uint8_t lap;
    uint8_t percent;
    bool isWinner;
} OnePlayer;

#endif