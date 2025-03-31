#ifndef HARDWARE_CONFIGURATION_h
#define HARDWARE_CONFIGURATION_h

#include "Arduino.h"
#include "./../../common/Common.h"

#define SOUND_OFF true

/*
    ******************************************************************
        Hardware
    ******************************************************************
*/

// #define NUMBER_OF_LED 64

// LED Strip
// =========
//
//  For WS2812 LED Strip:
//   _______  __                          ____[WS2812 LED Strip connector]____
//  |__Arduino_ |                        |                                    |
//  |    +5V    |>---------------------->| V+  (usually Red cable)            |
//  |    GND    |>---------------------->| GND (usually White cable)          |
//  |  PIN_LED  |>---->[R 500 ohms]----->| DI  (data in - usually Green cable)|
//   \_________/                          \__________________________________/
//
//
//  For WS2813 LED Strip:
//     WS2813 have a 4th cable: Backup data lines (BI- usually Blue cable)
//     Connect BI cable to GND
//   ___________                          ____[WS2813 LED Strip connector]_____
//  |__Arduino_ |                        |                                     |
//  |    +5V    |>---------------------->| V+  (usually Red cable)             |
//  |    GND    |>----------------o----->| GND (usually White cable)           |
//  |           |                  \---->| BI  (backup in - usually Blue cable)|
//  |  PIN_LED  |>---->[R 500 ohms]----->| DI  (data in - usually Green cable) |
//   \_________/                          \___________________________________/
//

// Loudspeaker
// ===========
//
//   ___________                          _____________
//  |__Arduino_ |                        |             |
//  |    GND    |>--------------------->o| Loudspeaker |
//  | PIN_AUDIO |>----->[CAP 2uF]------>o|             |
//   \_________/                         |_____________|
//
///////////////////////////////////////////////////////////////////////////////
enum hw_setup
{
    PIN_LED = PIN_LEDSTRIP,
    PIN_AUDIO = PIN_AUDIOSPEAKER,
};

// Digital Controllers (Buttons)
// =============================
//
//   ________________
//  |____Arduino____ |            ________________
//  |      GND       |>-------->o| Button 1 (Red) |
//  | DIG_CTRL_1_PIN |>-------->o|________________|
//  |                |            __________________
//  |      GND       |>-------->o| Button 2 (Green) |
//  | DIG_CTRL_2_PIN |>-------->o|__________________|
//  |                |            _________________
//  |      GND       |>-------->o| Button 3 (Blue) |
//  | DIG_CTRL_3_PIN |>-------->o|_________________|
//  |                |            __________________
//  |      GND       |>-------->o| Button 4 (White) |
//  | DIG_CTRL_4_PIN |>-------->o|__________________|
//   \______________/
//
#define DIG_CTRL_1_PIN PIN_PLAYER_1 // switch player 1 to PIN and GND
#define DIG_CTRL_2_PIN PIN_PLAYER_2 // switch player 2 to PIN and GND
#define DIG_CTRL_3_PIN PIN_PLAYER_3 // switch player 3 to PIN and GND
#define DIG_CTRL_4_PIN PIN_PLAYER_4 // switch player 4 to PIN and GND

#endif