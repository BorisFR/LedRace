#ifndef COMMON_H
#define COMMON_H
#pragma once

// #define PROTO_DEMO
#ifdef PROTO_DEMO
#define NUMBER_OF_LED 64
#define PIN_LEDSTRIP 22
#define BRIGHTNESS_LED 84
#define PIN_AUDIOSPEAKER 23
#define MAX_PLAYER_NUMBER 4
#define PIN_PLAYER_1 12
#define PIN_PLAYER_2 14
#define PIN_PLAYER_3 27
#define PIN_PLAYER_4 26
#define PIN_TYPE INPUT_PULLUP

#define RAMP_START 29
#define RAMP_CENTER 40
#define RAMP_END 50
#define RAMP_HEIGHT 2

#else

#define NUMBER_OF_LED 83
#define PIN_LEDSTRIP 13
#define BRIGHTNESS_LED 100
#define PIN_AUDIOSPEAKER 25
#define MAX_PLAYER_NUMBER 4
#define PIN_PLAYER_1 22
#define PIN_PLAYER_2 23
#define PIN_PLAYER_3 27
#define PIN_PLAYER_4 26
#define PIN_TYPE_PLAYER_1 INPUT_PULLUP
#define PIN_TYPE_PLAYER_2 INPUT_PULLUP
#define PIN_TYPE_PLAYER_3 INPUT_PULLUP
#define PIN_TYPE_PLAYER_4 INPUT_PULLUP

#define RAMP_START 58  // 174
#define RAMP_CENTER 62 // 188
#define RAMP_END 67    // 202
#define RAMP_HEIGHT 4
#define LOOP_START 3
#define LOOP_CENTER 13
#define LOOP_END 22
#define LOOP_HEIGHT 4

#endif

#define RACE_LOOP_NUMBER 5

#define SERIAL_BAUD 115200

#define EOL '\n' // End of Command char used in Protocol

#define REC_COMMAND_BUFLEN 32 // received command buffer size
                              // At the moment, the largest received command is RAMP CONFIGURATION (A)
                              //    ex: A1400,1430,1460,12,0[EOC] (for a 1500 LED strip)
                              // 21 CHAR

#define TX_COMMAND_BUFLEN 48 // send command buffer size
                             // At the moment, the largest send command is Q
                             //    ex: QTK:1500,1500,0,-1,60,0,0.006,0.015,1[EOC] (for a 1500 LED strip)
                             // 37 CHAR

#define ADDRESS_LENGTH 6
/*
#define UNIQUE_KEY 42
#define NAME_MAX_LENGTH 20

enum class StateDevice : uint8_t
{
    Start = 0,
    Available = 1,
    Occupied = 2,
    Absent = 3
};

struct tOneDevice
{
    uint8_t address[ADDRESS_LENGTH];
    char name[NAME_MAX_LENGTH];
    StateDevice state;
    unsigned long lastHeartbeat;
    uint8_t key = UNIQUE_KEY;
};
*/
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    char message[TX_COMMAND_BUFLEN];
    /*char a[32];
    int b;
    float c;
    bool d;*/
} struct_message;

// MAC Address of responder (the race's server)
#define BROADCAT_ADDRESS const uint8_t THE_ADDRESS[] = {0x88, 0x13, 0xBF, 0x01, 0xE4, 0xC0};
#define WIFI_CHANNEL 12
#endif