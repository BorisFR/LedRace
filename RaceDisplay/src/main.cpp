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

void discoverLedRace()
{
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
}

/* ***************************************************************************
  Display stuff
*************************************************************************** */

#include "Display.h"

Display display = Display();

/* ***************************************************************************
  Race stuff
*************************************************************************** */
#include "enums.h"

#define TIMEOUT_RACE_TO_TITLE 1000 * 60 * 2

DISPLAY_STATES states = WELCOME;
String version = "0.0.0";
String uid = "xxx";
uint8_t numberOfPlayers = 2; // MAX_PLAYER_NUMBER

OnePlayer players[MAX_PLAYER_NUMBER];
int percent;
bool refresh = false;
unsigned long last;
byte totalLeds = 0;
byte totalLaps = 0;
byte countdown;

void raceInit()
{
  for (byte i = 0; i < MAX_PLAYER_NUMBER; i++)
  {
    players[i].isWinner = false;
    players[i].lap = 1;
    players[i].percent = 0;
    players[i].percentTotal = 0;           //(i + 1) * 50;
    players[i].best = 1000 * 60 * 60 * 24; // 1 day :)
    players[i].bestLap = "--:--.---";
    players[i].total = 0;
    players[i].totalLap = players[i].bestLap;
    players[i].rank = 0;
    players[i].startLap = millis();
    players[i].displayInit = false;
  }
}

void ledRaceSetup()
{
  for (byte i = 0; i < MAX_PLAYER_NUMBER; i++)
  {
    players[i] = OnePlayer();
    players[i].positionX = PLAYER_PADDING_X;
    players[i].positionY = DISPLAY_PLAYERS_PADDING_Y + i * (DISPLAY_PLAYER_HEIGHT + DISPLAY_PLAYERS_DELTA_Y);
  }
  players[0].color = TFT_RED;
  players[1].color = TFT_BLUE;
  players[2].color = TFT_GREEN;
  players[3].color = TFT_WHITE;
  raceInit();
}

void raceStart()
{
  display.Clear();
  for (byte i = 0; i < numberOfPlayers; i++)
  {
    display.ShowPlayer(players[i]);
  }
}

String on2(uint8_t value)
{
  if (value > 9)
    return String(value);
  return "0" + String(value);
}

String on3(uint16_t value)
{
  if (value > 99)
    return String(value);
  if (value > 9)
    return "0" + String(value);
  return "00" + String(value);
}

String millisToString(unsigned long value)
{
  uint16_t seconds = value / 1000;
  int milliseconds = value - seconds * 1000;
  uint16_t minutes = seconds / 60;
  seconds = seconds - minutes * 60;
  return on2(minutes) + ":" + on2(seconds) + "." + on3(milliseconds);
}

void ranking()
{
  if (players[0].percentTotal > players[1].percentTotal)
  {
    if (players[0].rank == 1)
      return;
    players[0].rank = 1;
    players[1].rank = 2;
  }
  else
  {
    if (players[1].rank == 1)
      return;
    players[1].rank = 1;
    players[0].rank = 2;
  }
  display.ShowRank(players[0]);
  display.ShowRank(players[1]);
}

void parsePlayerLap(byte playerNumber, byte currentLap, byte position)
{
  if (players[playerNumber].lap != currentLap)
  {
    unsigned long time = millis();
    players[playerNumber].lap = currentLap;
    unsigned long duration = time - players[playerNumber].startLap;
    players[playerNumber].total += duration;
    if (players[playerNumber].best > duration)
    {
      players[playerNumber].best = duration;
      /*uint16_t seconds = duration / 1000;
      int milliseconds = duration - seconds * 1000;
      uint16_t minutes = seconds / 60;
      seconds = seconds - minutes * 60;
      players[playerNumber].bestLap = on2(minutes) + ":" + on2(seconds) + "." + on3(milliseconds);*/
      players[playerNumber].bestLap = millisToString(duration);
      display.ShowBest(players[playerNumber]);
    }
    players[playerNumber].totalLap = millisToString(players[playerNumber].total);
    display.ShowTotal(players[playerNumber]);
    players[playerNumber].startLap = millis();
  }
  if (players[playerNumber].percent != position)
  {
    players[playerNumber].percent = position;
    // totalLaps => 100
    // 1 => 100 / totalLaps
    percent = (((currentLap - 1) * 100.0) / totalLaps) + ((position * 1.0) / 10.0);
    players[playerNumber].percentTotal = percent;
    display.ShowPercent(players[playerNumber]);
    ranking();
  }
  // Serial.println("Player " + String(playerNumber) + ": " + String(currentLap) + " - " + String(percent) + "%");
}

void parsePlayerWin(byte playerNumber)
{
  parsePlayerLap(playerNumber, totalLaps + 1, 0);
  display.ShowWinner(players[playerNumber]);
  // Serial.println("Winner: " + String(playerNumber));
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(SERIAL_BAUD);
  display.Setup();
  delay(1000);
  ledRaceSetup();
  display.ShowTitle();
  discoverLedRace();
}

void loop()
{
  if (millis() - last > TIMEOUT_RACE_TO_TITLE)
  {
    display.ShowTitle();
    discoverLedRace();
    last = millis();
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

  while (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    switch (incomingByte)
    {
    case '5':
      break;
    default:
      Serial.println(incomingByte);
      break;
    }
  }
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
          uid = oneString.substr(1).c_str();
          display.ShowName(uid);
          break;
        case '?': // ?A2P0
          break;
        case '%': // %1.2.0
          version = oneString.substr(1).c_str();
          display.ShowVersion(version);
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
          parsePlayerLap(0, stoi(stringParts[2]), stoi(stringParts[3]));
          // Serial.println(stoi(stringParts[2]));
          // Serial.println(stoi(stringParts[3]));
        }
        else if (oneString == "p2")
        {
          parsePlayerLap(1, stoi(stringParts[2]), stoi(stringParts[3]));
        }
        else if (oneString == "p3")
        {
          parsePlayerLap(2, stoi(stringParts[2]), stoi(stringParts[3]));
        }
        else if (oneString == "p4")
        {
          parsePlayerLap(3, stoi(stringParts[2]), stoi(stringParts[3]));
        }
        else if (oneString == "w1")
        {
          parsePlayerWin(0);
        }
        else if (oneString == "w2")
        {
          parsePlayerWin(1);
        }
        else if (oneString == "w3")
        {
          parsePlayerWin(2);
        }
        else if (oneString == "w4")
        {
          parsePlayerWin(3);
        }
        else if (oneString == "c1")
        {
          countdown = 6;
          display.Countdown(countdown--);
        }
        else if (oneString == "c2")
        {
          display.Countdown(countdown--);
        }
        else if (oneString == "c3")
        {
          display.Countdown(countdown--);
        }
        else if (oneString == "c4")
        {
          display.Countdown(countdown--);
        }
        else if (oneString == "c5")
        {
          display.Countdown(countdown--);
        }
        else if (oneString == "c6")
        {
          display.Countdown(countdown--);
        }
        else if (oneString == "c7")
        {
          display.Countdown(countdown--);
        }
        else if (oneString == "c8")
        {
          // display.Countdown(countdown--);
        }
        else if (oneString == "c9")
        {
          // display.Countdown(0);
        }
        else if (oneString == "R3") // never seen
        {
          Serial.println("Ready to race");
        }
        else if (oneString == "R4")
        {
          // Serial.println("Countdown started");
          display.Countdown(3);
        }
        else if (oneString == "R5")
        {
          // Serial.println("GO!");
          raceInit();
          raceStart();
        }
        else if (oneString == "R8")
        {
          Serial.println("Race complete");
        }
        else if (oneString == "QTK")
        {
          totalLeds = stoi(stringParts[1]);
          Serial.println("Number of LED: " + String(totalLeds));
        }
        else if (oneString == "QRC")
        {
          totalLaps = stoi(stringParts[2]);
          Serial.println("Number of laps: " + String(totalLaps));
        }
        else if (oneString == "QRP")
        {
          // QRP,29,40,50,2,1
        }
        else if (oneString == "QBT")
        {
          // QBT,3,60,10,0
        }
        else if (oneString == "NOK")
        {
          // NOTHING
        }
        else
        {
          message = oneString.c_str();
          if (message.indexOf("Hello (") == 0)
          {
            display.ShowTitle();
            discoverLedRace();
          }
          else
          {
            Serial.println(txtBuffer);
          }
        }
        // message = "";
      } // incomingByte == EOL
    }
  } // While Serial2.available
}
