#include "Track.h"

Track::Track()
{
    FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, MAXLED);
    FastLED.setBrightness(84);
}

uint16_t Track::numPixels()
{
    return MAXLED;
}

uint32_t Track::Color(int red, int green, int blue)
{
    return (uint32_t)CRGB(red, green, blue);
}

void Track::setPixelColor(int index, uint32_t color)
{
    leds[index] = (CRGB)color;
}

void Track::begin()
{
}

void Track::show()
{
    FastLED.show();
}