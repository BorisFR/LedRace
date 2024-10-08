#include "Display.h"

Display::Display() {}

void Display::Setup()
{
    display.init();
    display.setRotation(0);
    display.fillScreen(TFT_BLACK);
}

void Display::Clear()
{
    display.fillScreen(TFT_BLACK);
}

void Display::Countdown(byte value)
{
    if (value == 0)
    {
        display.setFreeFont(&Orbitron_Bold_80);
        display.setTextColor(TFT_GREEN);
        display.setTextDatum(TC_DATUM);
        display.drawString("GO", TFT_WIDTH / 2, COUNTDOWN_POS_Y);
    }
    else
    {
        display.setFreeFont(&Orbitron_Bold_80);
        display.setTextColor(TFT_WHITE);
        display.setTextDatum(TC_DATUM);
        display.drawString(String(value), TFT_WIDTH / 2, COUNTDOWN_POS_Y);
    }
}

void Display::TotalLap(byte value)
{
    display.setFreeFont(&Roboto_Black_30);
    display.setTextDatum(TL_DATUM);
    display.setTextColor(TFT_CYAN);
    display.drawString("LAP: " + String(value), 6, 6);
}

void Display::PlayerLap(byte player, byte value)
{
}