#ifndef AUDIO_H
#define AUDIO_H

#include "Configuration.h"
#include "Parameters.h"

/// @brief All stuff for audio
class Audio
{
public:
    Audio();
    void SoundOff();
    void ChangeAudioMode();
    void PlayCountdown(countdown phase);
    void PlayMotorSound(unsigned int frequency);
    void PlayWinnerMusic();

private:
};

#endif