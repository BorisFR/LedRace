#ifndef _OPEN_LED_RACE_h
#define _OPEN_LED_RACE_h

#ifdef __cplusplus

extern "C"
{
#endif

#include "olr-lib.h"

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Definitons related to Serial Communication Protocol
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // !!! A change in definitions below requires a corresponding modification in:
    // !!!   - Protocol definition doc
    // !!!   - Every Host Software used with this Firmware (Configuration App, Network Race Enabler, ...)
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum loglevel
    { // used in Serial Protocol "!" command (send log/error message)
        LOG = 1,
        WARNING = 2,
        ERROR = 3
    };

    // Race Phases
    enum phases
    {
        IDLE = 0,
        CONFIG,
        CONFIG_OK,
        READY,
        COUNTDOWN,
        RACING,
        PAUSE,
        RESUME,
        COMPLETE,
        RACE_PHASES
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // Types for ack.type
    enum resp
    { //
        NOK = -1,
        NOTHING = 0,
        OKK = 1 // TODO: add a 'K' because conflict
    };

    // Answer sent after processing a command received from Host
    typedef struct ack
    {
        enum resp rp;
        char type;
    } ack_t;

    typedef struct cfgcircuit
    {
        uint8_t outtunnel;
    } cfgcircuit_t;

    typedef struct race
    {
        OneRace cfg;
        cfgcircuit_t circ;
        bool newcfg;
        enum phases phase;
        byte numcars;
        byte winner;
        bool demo_mode;
        bool demo_mode_on_received;
        bool demo_mode_off_received;
        bool network_race;
    } TheRace;

    /*  ----------- Function prototypes ------------------- */

    void sendResponse(ack_t *ack);

    ack_t manageSerialCommand();

    void printdebug(const char *msg, int errlevel);
    void print_cars_positions(OneCar *cars);
    void run_racecycle(void);
    void draw_winner(TheTrack *tck, uint32_t color);

#ifdef __cplusplus
} // extern "C"
#endif

#endif