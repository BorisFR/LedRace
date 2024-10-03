#ifndef GAME_CONFIGURATION_H
#define GAME_CONFIGURATION_H

#define MAX_PLAYER_NUMBER 4

/// LED number in the Strip used as a Semaphore (Countdown phase)
#define LED_SEMAPHORE 2

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