#include <Arduino.h>

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

TFT_eSPI display = TFT_eSPI(); // Invoke custom library

void setup()
{
  Serial.begin(115200);
  display.init();
  display.setRotation(2);
  display.fillScreen(TFT_BLACK);
  display.setTextDatum(C_BASELINE);
  // display.setFreeFont(&Roboto_14);
  display.setTextColor(TFT_VIOLET);
  display.drawString("ADIMAKER", TFT_WIDTH / 2, 15);
}

void loop()
{
  // display.setCursor(0, 0, 2);
  //  Set the font colour to be white with a black background, set text size multiplier to 1
  // display.setTextColor(TFT_WHITE, TFT_BLACK);
  // display.setTextSize(1);
  //  We can now plot text on screen using the "print" class
  // display.println("Hello World!");
  //  tft.setTextColor(63 << 5, 0);
  //  tft.drawCentreString("MATRIX", TFT_WIDTH / 2, TFT_HEIGHT / 2, 4);
}
