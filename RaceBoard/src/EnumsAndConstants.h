#ifndef ENUMS_AND_CONSTANTS_h
#define ENUMS_AND_CONSTANTS_h

#include "Arduino.h"

// Position (bit) into the ConfigurationParameter.option byte for On|Off settings
// Used in param_option_set()/param_option_is_active() calls
enum cfgparam_option_bit
{
    BATTERY_MODE_OPTION = 0,
    AUTOSTART_MODE_OPTION = 1,
    BOX_MODE_OPTION = 2,
    SLOPE_MODE_OPTION = 3,
    PLAYER_3_OPTION = 4,
    PLAYER_4_OPTION = 5,
    DEMO_MODE_OPTION = 6,
    NOT_USED_7_OPTION = 7,
};

enum ConfigurationParameterDefinition
{
    CFGPARAM_VER = 10, // Change this value (+=1) every time the [ConfigurationParameter] struct is modified
                       // This will force an update with the new [struct] to the settings
                       // stored in EEPROM with an old (invalid) struct
    LEN_UID = 16,
};

typedef struct cfgrace
{
    bool startline; // Standalone mode: Always 1
    uint8_t nlap;
    uint8_t nrepeat; // Standalone mode: Always 1
    bool finishline; // Standalone mode: Always 1
} OneRace;

struct ConfigurationBattery
{                  // added in ver 0.9.7
    uint8_t delta; // unsigned char value [1-254] / will be divided by 100 [0.01-2.54]
    uint8_t min;   // Battery charge does not goes below this "min" percentage
    uint8_t speed_boost_scaler;
};

struct ConfigurationTrack
{
    int nled_total;
    int nled_main;
    int nled_aux;
    int init_aux;
    int box_len; // used to hold the Box Length if the default is changed.
                 // it's not possible to implicitly store it in nled_main,nled_aux
                 // because, if these are different to the default, box gets always activated
                 // (the software does not chek "box_isactive" to draw car position)
    float kf;
    float kg;
};

// ramp centred in LED 100 with 10 led fordward and 10 backguard
struct ConfigurationRamp
{
    int init;
    int center;
    int end;
    uint8_t high;
};

struct BoardInfo
{
    char uid[LEN_UID + 1];
};

struct ConfigurationParameter
{
    uint8_t ver;    // Version of this [ConfigurationParameter] struct
    uint8_t option; // Bit-mapped byte to store 'active' on|off for options (Battery, AutoStart, BoxalwaysOn, etc)
    // struct cfgrace    race;  // added in ver 0.9.d
    OneRace race; // added in ver 0.9.d
    struct ConfigurationBattery battery;
    struct ConfigurationTrack track;
    struct ConfigurationRamp ramp;
    struct BoardInfo info;
};

enum trackType
{
    NOT_TRACK = 0,
    TRACK_MAIN,
    TRACK_AUX,
    TRACK_IN,
    TRACK_OUT,
    NUM_TRACKS,
};

typedef struct
{
    struct ConfigurationParameter cfg;
    int ledcoin; // LED_SPEED_COIN
    uint32_t ledtime;
    bool rampactive;
    bool boxactive;
} TheTrack;

enum carStatus
{
    CAR_WAITING = 0,
    CAR_COMING,
    CAR_ENTER,
    CAR_RACING,
    CAR_LEAVING,
    CAR_GO_OUT,
    CAR_FINISH
};

enum ControllerType
{
    NOT_DEFINED = 0,
    DIGITAL_MODE,
    ANALOG_MODE,
    DEMO_MODE,
};

enum ControllerIndex
{               // Used to access controller by "name" (and not via zero-offset index)
    CTRL_1 = 0, // Ex:  DIGITAL_CTRL[CTRL_2]
    CTRL_2,
    CTRL_3,
    CTRL_4
};

typedef struct
{
    enum ControllerType mode;
    int pin;
    int adc;
    int badc;
    int delta_analog;
    byte flag_sw;
} OneController;

typedef struct
{
    OneController *ct;
    float speed;
    float dist;
    float dist_aux;
    byte nlap;
    byte repeats;
    uint32_t color;
    uint8_t trackID;
    enum carStatus st;
    bool leaving;
    float battery;
    bool charging;
} OneCar;

enum stcoin
{
    COIN_RESET = -2,
    COIN_WAIT = -1,
};

#endif