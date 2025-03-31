#include "olr-param.h"

void param_setdefault(struct ConfigurationParameter *cfg, uint16_t numberOfLeds)
{
    cfg->ver = CFGPARAM_VER;

    cfg->race.startline = true;
    cfg->race.nlap = NUMLAP;
    cfg->race.nrepeat = 1;
    cfg->race.finishline = true;

    param_option_set(cfg, BATTERY_MODE_OPTION, BATTERY_MODE);
    cfg->battery.delta = 3;
    cfg->battery.min = 60;
    cfg->battery.speed_boost_scaler = 10;

    param_option_set(cfg, AUTOSTART_MODE_OPTION, AUTOSTART_MODE);

    /*cfg->ramp.init = 80;
    cfg->ramp.center = 90;
    cfg->ramp.end = 100;
    cfg->ramp.high = 6;
    param_option_set(cfg, SLOPE_MODE_OPTION, SLOPE_ALWAYS_ON);*/

    cfg->ramp.init = RAMP_START;
    cfg->ramp.center = RAMP_CENTER;
    cfg->ramp.end = RAMP_END;
    cfg->ramp.high = RAMP_HEIGHT;
    param_option_set(cfg, SLOPE_MODE_OPTION, SLOPE_ALWAYS_ON);

    cfg->track.nled_total = numberOfLeds; // NUMBER_OF_LED: Total LED number in the racetrack (default:300 -> 5mt, 60LED/mt Strip)
    cfg->track.nled_main = numberOfLeds;  // 240 when boxes length = 60
    cfg->track.nled_aux = 0;              // 60
    cfg->track.init_aux = -1;             // 239
    cfg->track.box_len = BOXLEN;
    param_option_set(cfg, BOX_MODE_OPTION, BOX_ALWAYS_ON);

    cfg->track.kf = CAR_FRICTION;
    // TODO: 0.015;                 // friction constant
    cfg->track.kg = CAR_SLOPE_GRAVITY;
    // TODO: 0.006; // gravity constant  - Used in Slope

    param_option_set(cfg, PLAYER_3_OPTION, PLAYER_3);
    param_option_set(cfg, PLAYER_4_OPTION, PLAYER_4);
    param_option_set(cfg, DEMO_MODE_OPTION, DEMO_MODE_ST);
    strcpy(cfg->info.uid, "ADIMAKER");
}

void param_option_set(struct ConfigurationParameter *cfg, uint8_t option, boolean value)
{
    bitWrite(cfg->option, option, value);
}
boolean param_option_is_active(struct ConfigurationParameter *cfg, uint8_t option)
{
    return (bitRead(cfg->option, option));
}
