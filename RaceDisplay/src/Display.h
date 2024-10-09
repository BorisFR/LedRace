#ifndef DISPLAY_h
#define DISPLAY_h

/*
DISPLAY
=======
LED   noir  3.3V    pjt temp
SCK   blanc 18 x    rouge   18
SDA   gris  23 x    marron  23
A0    mauve 2 x     noir    16
RESET bleu  4  x    jaune   4
CS    vert  15 x    orange  15
GND   jaune GND     marron
VCC   orange  5V    rouge

*/

#include "Arduino.h"

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "Orbitron-Bold-80.h"
#include "Roboto_Black_30.h"

#define COUNTDOWN_POS_Y 30

class Display
{
public:
    Display();
    void Setup();
    void Clear();
    void ShowVersion(String value);
    void ShowName(String value);
    void Countdown(byte value);
    void TotalLap(byte value);
    void PlayerLap(byte player, byte value);

private:
    TFT_eSPI display = TFT_eSPI();
    void PrintWithShadow(String text, int x, int y, uint16_t color);
};

#endif