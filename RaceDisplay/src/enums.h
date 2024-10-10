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
    uint8_t percentTotal;
    bool isWinner;
    unsigned long startLap;
    unsigned long best;
    String bestLap;
    unsigned long total;
    String totalLap;
    uint8_t rank;
    uint8_t positionX;
    uint8_t positionY;
    uint16_t color;
    bool displayInit;
} OnePlayer;

#endif