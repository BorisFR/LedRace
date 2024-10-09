#include "Display.h"

Display::Display() {}

void Display::Setup()
{
    display.init();
    display.setRotation(2);
    Clear();
    display.setFreeFont(&Roboto_Black_30);
    PrintWithShadow("LED", TFT_WIDTH / 2, 3 * TFT_HEIGHT / 8, TFT_RED);
    PrintWithShadow("RACE", TFT_WIDTH / 2, 5 * TFT_HEIGHT / 8, TFT_BLUE);
}

void Display::ShowName(String value)
{
    display.setFreeFont(&FreeSansBold12pt7b);
    display.setTextDatum(TC_DATUM);
    display.setTextColor(TFT_WHITE);
    display.setTextColor(TFT_WHITE);
    display.drawString(value, TFT_WIDTH / 2, 0);
}

void Display::ShowVersion(String value)
{
    display.setFreeFont(&FreeMono9pt7b);
    display.setTextDatum(BC_DATUM);
    display.setTextColor(TFT_WHITE);
    display.setTextColor(TFT_WHITE);
    display.drawString(value, TFT_WIDTH / 2, TFT_HEIGHT);
}

void Display::PrintWithShadow(String text, int x, int y, uint16_t color)
{
    display.setTextDatum(MC_DATUM);
    display.setTextColor(TFT_WHITE);
    for (int8_t i = -2; i < 3; i++)
        for (int8_t j = -2; j < 3; j++)
            display.drawString(text, x + i, y + j);
    display.setTextColor(color);
    display.drawString(text, x, y);
}

void Display::Clear()
{
    display.fillScreen(TFT_BLACK);
}

void Display::Countdown(byte value)
{
    Clear();
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