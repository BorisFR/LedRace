#include <Arduino.h>
#include "./../../common/Common.h"

/* ***************************************************************************
  Communication stuff (Serial2, pin 16/17)
*************************************************************************** */
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

int incomingByte;
stringstream stringToParse;
vector<string> stringParts;
char delimiter = ',';
string oneString;
String message;
char txtBuffer[TX_COMMAND_BUFLEN];
byte bufferPosition = 0;
char sendbuffer[TX_COMMAND_BUFLEN];
char commandReceived;

/* ***************************************************************************
  Display stuff
*************************************************************************** */

#include "Display.h"

Display display = Display();

/* ***************************************************************************
  Race stuff
*************************************************************************** */
unsigned long last;
byte countdownNumber;
byte lapNumber;

void showLap()
{
  display.PlayerLap(1, lapNumber);
}

void parsePlayerLap(byte playerNumber, byte currentLap, byte position)
{
  Serial.println("Player " + String(playerNumber) + ": " + String(currentLap) + " - " + String(position) + "%");
}

void parsePlayerWin(byte playerNumber)
{
  Serial.println("Winner: " + String(playerNumber));
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(SERIAL_BAUD);
  display.Setup();
  delay(1000);
  sprintf(sendbuffer, "#%c", EOL);
  Serial2.write(sendbuffer, 2);
  sprintf(sendbuffer, "$%c", EOL);
  Serial2.write(sendbuffer, 2);
  sprintf(sendbuffer, "?%c", EOL);
  Serial2.write(sendbuffer, 2);
  sprintf(sendbuffer, "\%%c", EOL);
  Serial2.write(sendbuffer, 2);
  sprintf(sendbuffer, "Q%c", EOL);
  Serial2.write(sendbuffer, 2);
  Serial2.write(sendbuffer, 2);

  countdownNumber = 0;
  lapNumber = 20;
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
    // display.Clear();
    // display.Countdown(countdownNumber);
    last = millis();
    // showLap();
  }
  // display.setCursor(0, 0, 2);
  //  Set the font colour to be white with a black background, set text size multiplier to 1
  // display.setTextColor(TFT_WHITE, TFT_BLACK);
  // display.setTextSize(1);
  //  We can now plot text on screen using the "print" class
  // display.println("Hello World!");
  //  tft.setTextColor(63 << 5, 0);
  //  tft.drawCentreString("MATRIX", TFT_WIDTH / 2, TFT_HEIGHT / 2, 4);
  // return;
  while (Serial2.available() > 0)
  {
    incomingByte = Serial2.read();
    if (incomingByte > 0 && incomingByte < 250)
    {
      if (bufferPosition >= TX_COMMAND_BUFLEN)
        bufferPosition = 0;
      txtBuffer[bufferPosition++] = (char)incomingByte;
      if (incomingByte == EOL)
      {
        txtBuffer[--bufferPosition] = 0;
        bufferPosition = 0;
        // oneString = txtBuffer;
        stringToParse = stringstream(txtBuffer);
        stringParts.clear();
        while (getline(stringToParse, oneString, delimiter))
          stringParts.push_back(oneString);
        oneString = stringParts[0].c_str();
        commandReceived = txtBuffer[0];
        switch (commandReceived)
        {
        case '$': // $ADIMAKER
          break;
        case '?': // ?A2P0
          break;
        case '%': // %1.2.0
          break;
        case 'Q': // Q...
                  /*
                  QTK:64,64,0,-1,10,0,0.002,0.008,0
          QRP:29,40,50,2,1
          QBT:3,60,10,0
          QRC:1,5,1,1,0,0,0,0
          */
          break;
        } // switch command
        if (oneString == "p1")
        {
          parsePlayerLap(1, stoi(stringParts[2]), stoi(stringParts[3]));
        }
        else if (oneString == "p2")
        {
          parsePlayerLap(2, stoi(stringParts[2]), stoi(stringParts[3]));
        }
        else if (oneString == "p3")
        {
          parsePlayerLap(3, stoi(stringParts[2]), stoi(stringParts[3]));
        }
        else if (oneString == "p4")
        {
          parsePlayerLap(4, stoi(stringParts[2]), stoi(stringParts[3]));
        }
        else if (oneString == "w1")
        {
          parsePlayerWin(1);
        }
        else if (oneString == "w2")
        {
          parsePlayerWin(2);
        }
        else if (oneString == "w3")
        {
          parsePlayerWin(3);
        }
        else if (oneString == "w4")
        {
          parsePlayerWin(4);
        }
        else if (oneString == "c1")
        {
          Serial.println("5");
        }
        else if (oneString == "c2")
        {
          Serial.println("4");
        }
        else if (oneString == "c3")
        {
          Serial.println("3");
        }
        else if (oneString == "c4")
        {
          Serial.println("2");
        }
        else if (oneString == "c5")
        {
          Serial.println("1");
        }
        else if (oneString == "R3") // never seen
        {
          Serial.println("Ready to race");
        }
        else if (oneString == "R4")
        {
          Serial.println("Countdown started");
        }
        else if (oneString == "R5")
        {
          Serial.println("GO!");
        }
        else if (oneString == "R8")
        {
          Serial.println("Race complete");
        }
        else if (oneString == "NOK")
        {
          // NOTHING
        }
        else
        {
          Serial.println(txtBuffer);
        }
        // message = "";
      } // incomingByte == EOL
    }
  } // While Serial2.available
}
