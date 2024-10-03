#ifndef LED_STRIP_h
#define LED_STRIP_h

#include <Arduino.h>
#include "HardwareConfiguration.h"
#include "FastLED.h"
#include "olr-param.h"

class LedStrip
{

public:
    LedStrip();
    void Setup();
    uint32_t Color(int red, int green, int blue);
    void SetPixelColor(int index, uint32_t color);
    uint16_t NumberOfPixels();
    void Show();
    void FillStrip(uint32_t c, uint8_t wait);

private:
    CRGB leds[NUMBER_OF_LED];
};

#endif