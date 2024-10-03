#include "Audio.h"

Audio::Audio() { playMotorSound = false; }

void Audio::SoundOff()
{
    noTone(PIN_AUDIO);
}

void Audio::MotorSound(bool isActivate)
{
    playMotorSound = isActivate;
    tone(PIN_AUDIO, 100);
}

void Audio::PlayMotorSound(unsigned int frequency)
{
    if (!playMotorSound)
        return;
    tone(PIN_AUDIO, frequency);
}
void Audio::PlayCountdown(countdown phase)
{
    switch (phase)
    {
    case Countdown1:
        tone(PIN_AUDIO, Countdown1Frequency);
        break;
    case Countdown2:
        tone(PIN_AUDIO, Countdown2Frequency);
        break;
    case Countdown3:
        tone(PIN_AUDIO, Countdown3Frequency);
        break;
    case Countdown4:
        tone(PIN_AUDIO, Countdown4Frequency);
        break;
    case Countdown5:
        SoundOff();
        break;
    }
}

void Audio::PlayWinnerMusic()
{
    int win_music[] = {
        2637, 2637, 0, 2637,
        0, 2093, 2637, 0,
        3136};
    int const msize = sizeof(win_music) / sizeof(int);
    for (int note = 0; note < msize; note++)
    {
        tone(PIN_AUDIO, win_music[note], 200);
        delay(WINNER_AUDIO_DELAY);
        noTone(PIN_AUDIO);
    }
}