#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Arduino.h"
#include "./../../common/Common.h"

/*
    ******************************************************************
        Hardware
    ******************************************************************
*/

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
    PIN_LED = 22,
    PIN_AUDIO = 23,
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
#define DIG_CTRL_1_PIN 12 // switch player 1 to PIN and GND
#define DIG_CTRL_2_PIN 14 // switch player 2 to PIN and GND
#define DIG_CTRL_3_PIN 27 // switch player 3 to PIN and GND
#define DIG_CTRL_4_PIN 26 // switch player 4 to PIN and GND

/*
    ******************************************************************
        Software
    ******************************************************************
*/

// Colors setup (Racing lights, Ramp, etc)

#define COLOR1 track.Color(255, 0, 0)     // Light controlled by DIG_CTRL_1_PIN
#define COLOR2 track.Color(0, 0, 255)     // Light controlled by DIG_CTRL_2_PIN
#define COLOR3 track.Color(0, 255, 0)     // Light controlled by DIG_CTRL_3_PIN
#define COLOR4 track.Color(255, 255, 255) // Light controlled by DIG_CTRL_4_PIN

#define COLOR_RAMP track.Color(64, 0, 64)
#define COLOR_COIN track.Color(40, 34, 0)
#define COLOR_BOXMARKS track.Color(64, 64, 0)
#define WARNING_BLINK_COLOR track.Color(32, 20, 0)

#endif