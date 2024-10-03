#include "Arduino.h"
#include "olr-lib.h"

void car_init(OneCar *car, OneController *ct, uint32_t color)
{
    car->ct = ct;
    car->color = color;
    car->trackID = TRACK_MAIN;
    car->speed = 0;
    car->dist = 0;
    car->dist_aux = 0;
    car->battery = 100;
    car->charging = 0;
}

void car_updateController(OneCar *car, float controllerSpeed)
{
    car->speed += controllerSpeed * car->battery / 100;
}

void update_track(OneTrack *tck, OneCar *car, byte controllerStatus, float acceleration)
{
    OneController *ct = car->ct;
    struct ConfigurationTrack const *cfg = &tck->cfg.track;

    if (car->trackID == TRACK_MAIN && (int)car->dist % cfg->nled_main == (cfg->init_aux - (cfg->nled_aux)) && controllerStatus == 0)
    { // change track by switch
        //  &&  (car->speed <= SPD_MIN_TRACK_AUX )) {   //change track by low speed

        car->trackID = TRACK_AUX;
        car->dist_aux = 0;
    }
    else if (car->trackID == TRACK_AUX && car->dist_aux > cfg->nled_aux)
    {

        car->trackID = TRACK_MAIN;
        car->dist += cfg->nled_aux;
    }

    /* Update car position in the current track */
    if (car->trackID == TRACK_AUX)
        process_aux_track(tck, car, acceleration);
    else if (car->trackID == TRACK_MAIN)
        process_main_track(tck, car, controllerStatus);

    /* Update car lap */
    if (car->dist > (cfg->nled_main * car->nlap - 1))
        car->nlap++;
}

void process_aux_track(OneTrack *tck, OneCar *car, float acceleration)
{
    struct ConfigurationTrack const *cfg = &tck->cfg.track;
    struct ConfigurationBattery const *battery = &tck->cfg.battery;

    // TODO: new line
    car->speed = 0.9 * car->speed;

    if ((int)car->dist_aux == tck->ledcoin && car->speed <= acceleration)
    {
        car->speed = acceleration * (1.0 * battery->speed_boost_scaler);

        tck->ledcoin = COIN_RESET;
        car->battery = 100;
    };

    car->speed -= car->speed * cfg->kf;
    car->dist_aux += car->speed;
}

void process_main_track(OneTrack *tck, OneCar *car, byte controllerStatus)
{
    struct ConfigurationTrack const *cfg = &tck->cfg.track;

    if (tck->rampactive)
    {
        struct ConfigurationRamp const *r = &tck->cfg.ramp;
        int const pos = (int)car->dist % cfg->nled_main;
        if (pos >= r->init && pos < r->center)
            //    car->speed -= cfg->kg * r->high * ( pos - r->init );
            car->speed -= cfg->kg * r->high;

        if (pos <= r->end && pos > r->center)
            // car->speed += cfg->kg * r->high * ( pos - r->center );
            car->speed += cfg->kg * r->high;
    }

    if (param_option_is_active(&tck->cfg, BATTERY_MODE_OPTION))
    { // Battery Mode ON
        struct ConfigurationBattery const *battery = &tck->cfg.battery;
        if (cfg->nled_main - (int)(car->dist) % cfg->nled_main == tck->ledcoin && controllerStatus == 0 // charge battery by push switch over coin
                                                                                                        //&& car->speed <= controller_getAccel()
        )
        {
            car->charging = 1;
        };

        if (car->charging == 1)
        {
            car->battery += battery->delta / 100.0 * 2;

            car->speed = 0;
            if (car->battery > 100)
            {
                tck->ledcoin = COIN_RESET;
                car->battery = 100;
                car->charging = 0;
                // car->speed = controller_getAccel()*SPEED_BOOST_SCALER;
                car->speed = 0.1 * battery->speed_boost_scaler;
            };
        };

        if (car->ct->flag_sw == 0)
        {
            if ((car->battery) >= battery->min)
            {
                car->battery -= battery->delta / 100.0;
            };
        };
        if (car->ct->flag_sw == 1)
        {
            if (car->charging == 1)
            {
                car->charging = 0;
                car->speed = (0.1 * battery->speed_boost_scaler) / 2;
            };
        };
    };

    car->speed -= car->speed * cfg->kf;
    car->dist += car->speed;
}

void ramp_init(OneTrack *tck)
{
    tck->rampactive = true;
}

bool ramp_isactive(OneTrack *tck)
{
    return tck->rampactive;
}

void car_resetPosition(OneCar *car, bool reset_speed)
{

    car->trackID = TRACK_MAIN;
    if (reset_speed)
        car->speed = 0;
    car->dist = 0;
    car->dist_aux = 0;
    car->nlap = 1;
    car->leaving = false;
    car->battery = 100; // Todo: propagate car's battery status in relay races !!!
}

void car_setSpeed(OneCar *car, float speed)
{
    car->speed = speed;
}

void box_init(OneTrack *tck)
{
    tck->boxactive = true;
}

bool box_isactive(OneTrack *tck)
{
    return tck->boxactive;
}

int tracklen_configure(OneTrack *tck, int nled)
{
    struct ConfigurationTrack *cfg = &tck->cfg.track;
    if (nled <= 0)
        return -1;
    cfg->nled_total = nled;
    return 0;
}

int autostart_configure(OneTrack *tck, uint8_t autostart)
{
    param_option_set(&tck->cfg, AUTOSTART_MODE_OPTION, (boolean)autostart);
    return 0;
}

int demo_configure(OneTrack *tck, uint8_t demo)
{
    param_option_set(&tck->cfg, DEMO_MODE_OPTION, (boolean)demo);
    return 0;
}

int players_n_configure(OneTrack *tck, uint8_t val)
{
    switch (val)
    {
    case 2:
        param_option_set(&tck->cfg, PLAYER_3_OPTION, false);
        param_option_set(&tck->cfg, PLAYER_4_OPTION, false);
        break;

    case 3:
        param_option_set(&tck->cfg, PLAYER_3_OPTION, true);
        param_option_set(&tck->cfg, PLAYER_4_OPTION, false);
        break;

    case 4:
        param_option_set(&tck->cfg, PLAYER_3_OPTION, true);
        param_option_set(&tck->cfg, PLAYER_4_OPTION, true);
        break;

    default:
        return (-1);
    }
    return (0);
}

int boxlen_configure(OneTrack *tck, int box_len, int boxalwaysOn)
{
    struct ConfigurationTrack *cfg = &tck->cfg.track;

    if (boxalwaysOn != 0 && boxalwaysOn != 1)
        return -1;
    if (box_len <= 0 || box_len >= cfg->nled_total)
        return -1;
    cfg->box_len = box_len;
    // cfg->box_alwaysOn = boxalwaysOn;
    param_option_set(&tck->cfg, BOX_MODE_OPTION, (boolean)boxalwaysOn);
    // Update  track->boxactive
    tck->boxactive = boxalwaysOn;

    return 0;
}

int physic_configure(OneTrack *tck, float kgp, float kfp)
{
    struct ConfigurationTrack *cfg = &tck->cfg.track;

    if (kgp <= 0.0 || kgp >= 2.0)
        return -1;
    if (kfp <= 0.0 || kfp >= 2.0)
        return -1;
    cfg->kf = kfp;
    cfg->kg = kgp;
    return (0);
}

int track_configure(OneTrack *tck, int init_box)
{
    struct ConfigurationTrack *cfg = &tck->cfg.track;

    if (init_box >= cfg->nled_total)
        return -1;
    cfg->nled_main = (init_box == 0) ? cfg->nled_total : init_box;
    cfg->nled_aux = (init_box == 0) ? 0 : cfg->nled_total - init_box;
    cfg->init_aux = init_box - 1;
    return 0;
}

int ramp_configure(OneTrack *tck, int init, int center, int end, uint8_t high, int alwaysOn)
{
    struct ConfigurationRamp *ramp = &tck->cfg.ramp;

    if (init >= tck->cfg.track.nled_main || init <= 0)
        return -1;
    if (center >= tck->cfg.track.nled_main || center <= 0)
        return -2;
    if (end >= tck->cfg.track.nled_main || end <= 0)
        return -3;
    if (!(center > init && center < end))
        return -4;
    if (alwaysOn != 0 && alwaysOn != 1)
        return -5;

    ramp->init = init;
    ramp->center = center;
    ramp->end = end;
    ramp->high = high;

    param_option_set(&tck->cfg, SLOPE_MODE_OPTION, (boolean)alwaysOn);
    // Update  track->rampactive
    tck->rampactive = alwaysOn;

    return 0;
}

int battery_configure(OneTrack *tck, int delta, int min, int boost, int active)
{
    struct ConfigurationBattery *battery = &tck->cfg.battery;

    battery->delta = delta;
    battery->min = min;
    battery->speed_boost_scaler = boost;
    param_option_set(&tck->cfg, BATTERY_MODE_OPTION, (boolean)active);

    return 0;
}

int race_configure(OneTrack *tck, int startline, int nlap, int nrepeat, int finishline)
{
    struct cfgrace *race = &tck->cfg.race;

    if (startline != 0 && startline != 1)
        return -1;
    if (finishline != 0 && finishline != 1)
        return -1;
    race->startline = startline;
    race->finishline = finishline;
    race->nlap = nlap;
    race->nrepeat = nrepeat;
    return 0;
}
