#include "LedStrip.h"

LedStrip::LedStrip()
{
    FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, NUMBER_OF_LED);
    FastLED.setBrightness(BRIGHTNESS_LED);
}

void LedStrip::Setup()
{
    // FillStrip(track.Color(255, 0, 0), 50);                  // Red
    // FillStrip(track.Color(0, 255, 0), 50);                  // Green
    // FillStrip(track.Color(0, 0, 255), 50);                  // Blue
}

uint16_t LedStrip::NumberOfPixels()
{
    return NUMBER_OF_LED;
}

uint32_t LedStrip::Color(int red, int green, int blue)
{
    return (uint32_t)CRGB(red, green, blue);
}

void LedStrip::SetPixelColor(int index, uint32_t color)
{
    leds[index] = (CRGB)color;
}

void LedStrip::Show()
{
    FastLED.show();
}

// Fill the dots one after the other with a color
void LedStrip::FillStrip(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < NumberOfPixels(); i++)
    {
        SetPixelColor(i, c);
        Show();
        delay(wait);
    }
}