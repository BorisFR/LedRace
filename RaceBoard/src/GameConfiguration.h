#ifndef GAME_CONFIGURATION_H
#define GAME_CONFIGURATION_H

#define MAX_PLAYER_NUMBER 4

/// LED number in the Strip used as a Semaphore (Countdown phase)
#define LED_SEMAPHORE 2

#define CAR_ACCELERATION 0.20   // 0.15
#define CAR_FRICTION 0.008      // 0.015
#define CAR_SLOPE_GRAVITY 0.002 // 0.006

#define SPD_MIN_TRACK_AUX 0.8 // change track by low speed
// #define BATTERY_DELTA           0.03  // Decrease BATTERY_DELTA on each controller activation - used in charge rate too
// #define BATTERY_DELTA           3     // unsigned char value [1-254] / will be divided by 100 [0.01-2.54]
// #define BATTERY_MIN             60    // Battery does not descharge below BATTERY_MIN
// #define SPEED_BOOST_SCALER      10

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

// Colors setup (Racing lights, Ramp, etc)

#define CAR_COLOR_1 track.Color(255, 0, 0)
#define CAR_COLOR_2 track.Color(0, 0, 255)
#define CAR_COLOR_3 track.Color(0, 255, 0)
#define CAR_COLOR_4 track.Color(255, 255, 255)

#define COLOR_RAMP track.Color(64, 0, 64)
#define COLOR_COIN track.Color(40, 34, 0)
#define COLOR_BOXMARKS track.Color(64, 64, 0)
#define WARNING_BLINK_COLOR track.Color(32, 20, 0)

#endif