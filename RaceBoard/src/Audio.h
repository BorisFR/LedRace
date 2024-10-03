#ifndef AUDIO_H
#define AUDIO_H

#include "HardwareConfiguration.h"
#include "Parameters.h"

/// @brief All stuff for audio
class Audio
{
public:
    Audio();
    void SoundOff();
    void MotorSound(bool isActivate);
    void PlayMotorSound(unsigned int frequency);
    void PlayCountdown(countdown phase);
    void PlayWinnerMusic();

private:
    bool playMotorSound = false;
};

#endif