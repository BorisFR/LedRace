#ifndef DISPLAY_h
#define DISPLAY_h

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

*/

#include "Arduino.h"

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "Orbitron-Bold-80.h"
#include "Roboto_Black_30.h"
#include "enums.h"

#define COUNTDOWN_POS_Y 30
#define PLAYER_PADDING_X 48
#define DISPLAY_PLAYERS_PADDING_Y 10
#define DISPLAY_PLAYERS_DELTA_Y 10

#define DISPLAY_PLAYER_ROUND_RADIUS 8
#define PROGRESS_BAR_PADDING_X 8
#define PROGRESS_BAR_Y 8
#define PROGRESS_BAR_HEIGHT 12
#define PROGRESS_BARFILL_PADDING 1
#define PROGRESS_BAR_PERCENT_HEIGHT 10
#define DISPLAY_PLAYER_WIDTH 80 // TFT_WIDTH - PLAYER_PADDING_X * 2
#define DISPLAY_PLAYER_HEIGHT 58
#define BEST_X 4
#define BEST_PADDING_X 4
#define BEST_Y PROGRESS_BAR_Y + PROGRESS_BAR_HEIGHT + 2
#define TIME_WIDTH 64
#define TIME_HEIGHT 14
#define TOTAL_X BEST_X
#define TOTAL_Y BEST_Y + TIME_HEIGHT
#define RANK_X -30
#define RANK_Y 10
#define RANK_WIDTH 20
#define RANK_HEIGHT 25
#define RANK_WINNER_X RANK_X + 8  //+ RANK_WIDTH / 2
#define RANK_WINNER_Y RANK_Y + 14 //+ RANK_WIDTH / 2
#define RANK_WINNER_RADIUS 20

class Display
{
public:
    Display();
    void Setup();
    void Clear();
    void ShowTitle();
    void ShowVersion(String value);
    void ShowName(String value);
    void Countdown(byte value);
    void ShowPlayer(OnePlayer player);
    void ShowPercent(OnePlayer player);
    void ShowBest(OnePlayer player);
    void ShowTotal(OnePlayer player);
    void ShowWinner(OnePlayer player, bool reverseColor = false);
    // void ShowRank(OnePlayer player1, OnePlayer player2);
    void ShowRank(OnePlayer player, bool clear = true);

private:
    TFT_eSPI display = TFT_eSPI();
    void PrintWithShadow(String text, int x, int y, uint16_t color);
    uint16_t progressBarWidth;
};

#endif