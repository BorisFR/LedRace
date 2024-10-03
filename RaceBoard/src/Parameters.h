#ifndef PARAMETERS_h
#define PARAMETERS_h

enum countdown
{
    Countdown1 = 1,
    Countdown2 = 2,
    Countdown3 = 3,
    Countdown4 = 4,
    Countdown5 = 5,
};

enum countdownFrequency
{
    Countdown1Frequency = 400,
    Countdown2Frequency = 600,
    Countdown3Frequency = 1200,
    Countdown4Frequency = 880,
};
enum delays_setup
{                                      // If you have a custom hardware (i.e not the OLR PCB),
    CONTDOWN_PHASE_DURATION = 1500,    // (mSec)
    CONTDOWN_STARTSOUND_DURATION = 40, // (mSec)
    WINNER_AUDIO_DELAY = 200,          // (mSec)
    WINNER_SHOW_DELAY = 20,            // (mSec)
    WINNER_SHOW_DURATION = 500,        // (mSec)
    NEWRACE_DELAY = 5000,              // (mSec)
    INACTIVITY_TIMEOUT_DELAY = 300,    // (Sec)  When demo_mode is active, board goes into demo mode after this inactivity time
    TELEMETRY_DELAY = 250,             // (mSec) Telemetry data sent every TELEMETRY_DELAY mSec
};

#endif