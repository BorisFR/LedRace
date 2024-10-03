#ifndef TRACK_h
#define TRACK_h

#include <Arduino.h>

#include "FastLED.h"
#include "olr-settings.h"
#include "olr-param.h"

class Track
{

public:
    Track();
    uint32_t Color(int red, int green, int blue);
    void setPixelColor(int index, uint32_t color);
    uint16_t numPixels();
    void begin();
    void show();

private:
    CRGB leds[MAXLED];
};

#endif