#ifndef _OLR_LIB_h
#define _OLR_LIB_h

#ifdef __cplusplus

extern "C"
{
#endif

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "olr-param.h"
#include "EnumsAndConstants.h"

    void car_init(OneCar *car, OneController *ct, uint32_t color);

    void car_updateController(OneCar *car, float controllerSpeed);

    void car_resetPosition(OneCar *car, bool reset_speed);

    void update_track(OneTrack *tck, OneCar *car, byte controllerStatus, float acceleration);

    void process_main_track(OneTrack *tck, OneCar *car, byte controllerStatus);

    void process_aux_track(OneTrack *tck, OneCar *car, float acceleration);

    void box_init(OneTrack *tck);

    bool box_isactive(OneTrack *tck);

    int tracklen_configure(OneTrack *tck, int nled);

    int autostart_configure(OneTrack *tck, uint8_t autostart);

    int demo_configure(OneTrack *tck, uint8_t demo);

    int players_n_configure(OneTrack *tck, uint8_t val);

    int boxlen_configure(OneTrack *tck, int box_len, int boxalwaysOn);

    int physic_configure(OneTrack *tck, float kgp, float kfp);

    int track_configure(OneTrack *tck, int init_box);

    void ramp_init(OneTrack *tck);

    bool ramp_isactive(OneTrack *tck);

    int ramp_configure(OneTrack *tck, int init, int center, int end, uint8_t high, int alwaysOn);

    int battery_configure(OneTrack *tck, int delta, int min, int boost, int active);

    int race_configure(OneTrack *tck, int startline, int nlap, int nrepeat, int finishline);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
