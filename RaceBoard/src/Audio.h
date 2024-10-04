#ifndef AUDIO_H
#define AUDIO_H

#include "HardwareConfiguration.h"
#include "Parameters.h"

/// @brief All stuff for audio
class Audio
{
public:
    Audio();
    // Mute the audio
    void MuteOn();
    // Unmute the audio
    void MuteOff();
    // Stop the sound
    void SoundOff();
    // Activate motor's sound
    void MotorSound(bool isActivate);
    // Play the motor's sound
    void PlayMotorSound(unsigned int frequency);
    // Play the audio countdown
    void PlayCountdown(countdown phase);
    // play the winner sound
    void PlayWinnerMusic();

private:
    bool muteActive = false;
    bool playMotorSound = false;
};

#endif