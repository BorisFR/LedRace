#ifndef CAR_h
#define CAR_h

#include "Arduino.h"
#include "EnumsAndConstants.h"
#include "olr-param.h"

class Cars
{
public:
    Cars();

    void car_init(OneCar *car, OneController *ct, uint32_t color);
    void car_updateController(OneCar *car, float controllerSpeed);
    void car_resetPosition(OneCar *car, bool reset_speed);
    void update_track(TheTrack *tck, OneCar *car, byte controllerStatus, float acceleration);
    void process_main_track(TheTrack *tck, OneCar *car, byte controllerStatus);
    void process_aux_track(TheTrack *tck, OneCar *car, float acceleration);
    void box_init(TheTrack *tck);
    bool box_isactive(TheTrack *tck);
    int tracklen_configure(TheTrack *tck, int nled);
    int autostart_configure(TheTrack *tck, uint8_t autostart);
    int demo_configure(TheTrack *tck, uint8_t demo);
    int players_n_configure(TheTrack *tck, uint8_t val);
    int boxlen_configure(TheTrack *tck, int box_len, int boxalwaysOn);
    int physic_configure(TheTrack *tck, float kgp, float kfp);
    int track_configure(TheTrack *tck, int init_box);
    void ramp_init(TheTrack *tck);
    bool ramp_isactive(TheTrack *tck);
    int ramp_configure(TheTrack *tck, int init, int center, int end, uint8_t high, int alwaysOn);
    int battery_configure(TheTrack *tck, int delta, int min, int boost, int active);
    int race_configure(TheTrack *tck, int startline, int nlap, int nrepeat, int finishline);

private:
};

#endif