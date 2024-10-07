#include <Arduino.h>
#include "./../../common/Common.h"

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

SERIAL
======
U2/TX: 17
U2/RX: 16
*/

// #include <Wire.h>

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "Orbitron-Bold-80.h"
#include "Roboto_Black_30.h"

TFT_eSPI display = TFT_eSPI(); // Invoke custom library

unsigned long last;
byte countdownNumber;
byte lapNumber;

#define COUNTDOWN_POS_Y 30

void showLap()
{
  display.setFreeFont(&Roboto_Black_30);
  display.setTextDatum(TL_DATUM);
  display.setTextColor(TFT_CYAN);
  display.drawString("LAP " + String(lapNumber), 6, 6);
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(SERIAL_BAUD);
  display.init();
  display.setRotation(0);
  display.fillScreen(TFT_BLACK);

  countdownNumber = 0;
  lapNumber = 0;
  showLap();
}

void loop()
{
  if (millis() - last > 700)
  {
    if (countdownNumber > 0)
      countdownNumber--;
    else
      countdownNumber = 5;
    display.fillScreen(TFT_BLACK);
    if (countdownNumber == 0)
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
      display.drawString(String(countdownNumber), TFT_WIDTH / 2, COUNTDOWN_POS_Y);
    }
    last = millis();
    showLap();
  }
  // display.setCursor(0, 0, 2);
  //  Set the font colour to be white with a black background, set text size multiplier to 1
  // display.setTextColor(TFT_WHITE, TFT_BLACK);
  // display.setTextSize(1);
  //  We can now plot text on screen using the "print" class
  // display.println("Hello World!");
  //  tft.setTextColor(63 << 5, 0);
  //  tft.drawCentreString("MATRIX", TFT_WIDTH / 2, TFT_HEIGHT / 2, 4);
}
