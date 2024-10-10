#include "Display.h"

Display::Display() {}

void Display::Setup()
{
    display.init();
    display.setRotation(2);
    ShowTitle();
}

void Display::ShowTitle()
{
    Clear();
    display.setFreeFont(&Roboto_Black_30);
    PrintWithShadow("LED", TFT_WIDTH / 2, 3 * TFT_HEIGHT / 8, TFT_RED);
    PrintWithShadow("RACE", TFT_WIDTH / 2, 5 * TFT_HEIGHT / 8, TFT_BLUE);
    progressBarWidth = DISPLAY_PLAYER_WIDTH - 2 * (PROGRESS_BAR_PADDING_X + PROGRESS_BARFILL_PADDING);
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

void Display::ShowPercent(OnePlayer player)
{
    display.fillRect(player.positionX + PROGRESS_BAR_PADDING_X + PROGRESS_BARFILL_PADDING, player.positionY + PROGRESS_BAR_Y + PROGRESS_BARFILL_PADDING, player.percentTotal * progressBarWidth / 100, PROGRESS_BAR_PERCENT_HEIGHT, player.color);
    if (player.percentTotal < 99)
    {
        display.fillRect(player.positionX + PROGRESS_BAR_PADDING_X + PROGRESS_BARFILL_PADDING + player.percentTotal * progressBarWidth / 100, player.positionY + PROGRESS_BAR_Y + PROGRESS_BARFILL_PADDING, (100 - player.percentTotal) * progressBarWidth / 100, PROGRESS_BAR_PERCENT_HEIGHT, TFT_BLACK);
    }
}

void Display::ShowBest(OnePlayer player)
{
    display.setTextFont(2);
    display.setTextColor(TFT_WHITE);
    display.setTextDatum(TL_DATUM);
    display.fillRect(player.positionX + BEST_X + BEST_PADDING_X, player.positionY + BEST_Y, TIME_WIDTH, TIME_HEIGHT, TFT_BLACK);
    display.drawString(player.bestLap, player.positionX + BEST_X + BEST_PADDING_X, player.positionY + BEST_Y);
}

void Display::ShowTotal(OnePlayer player)
{
    display.setTextFont(2);
    display.setTextColor(TFT_WHITE);
    display.setTextDatum(TL_DATUM);
    display.fillRect(player.positionX + TOTAL_X + BEST_PADDING_X, player.positionY + TOTAL_Y, TIME_WIDTH, TIME_HEIGHT, TFT_BLACK);
    display.drawString(player.totalLap, player.positionX + TOTAL_X + BEST_PADDING_X, player.positionY + TOTAL_Y);
}

void Display::ShowPlayer(OnePlayer player)
{
    if (!player.displayInit)
    {
        player.displayInit = true;
        // round rectangle around the player
        display.fillRoundRect(player.positionX, player.positionY, DISPLAY_PLAYER_WIDTH, DISPLAY_PLAYER_HEIGHT, DISPLAY_PLAYER_ROUND_RADIUS, player.color);
        display.fillRoundRect(player.positionX + DISPLAY_PLAYER_ROUND_RADIUS / 2, player.positionY + DISPLAY_PLAYER_ROUND_RADIUS / 2, DISPLAY_PLAYER_WIDTH - DISPLAY_PLAYER_ROUND_RADIUS, DISPLAY_PLAYER_HEIGHT - DISPLAY_PLAYER_ROUND_RADIUS, DISPLAY_PLAYER_ROUND_RADIUS, TFT_BLACK);
        // progress bar for % complete
        display.drawRect(player.positionX + PROGRESS_BAR_PADDING_X, player.positionY + PROGRESS_BAR_Y, DISPLAY_PLAYER_WIDTH - PROGRESS_BAR_PADDING_X * 2, PROGRESS_BAR_HEIGHT, TFT_YELLOW);
    }
    ShowPercent(player);
    ShowBest(player);
    ShowTotal(player);
}

void Display::ShowRank(OnePlayer player, bool clear)
{
    display.setFreeFont(&Roboto_Black_30);
    display.setTextDatum(TL_DATUM);
    if (clear)
        display.fillRect(player.positionX + RANK_X, player.positionY + RANK_Y, RANK_WIDTH, RANK_HEIGHT, TFT_BLACK);
    display.setTextColor(player.color);
    display.drawString(String(player.rank), player.positionX + RANK_X, player.positionY + RANK_Y);
}

// void Display::ShowRank(OnePlayer player1, OnePlayer player2)
//{
//   PrintRank(player1);
// PrintRank(player2);
/*display.setFreeFont(&Roboto_Black_30);
display.setTextDatum(TL_DATUM);
display.fillRect(player1.positionX + RANK_X, player1.positionY + RANK_Y, RANK_WIDTH, RANK_HEIGHT, TFT_BLACK);
display.fillRect(player2.positionX + RANK_X, player2.positionY + RANK_Y, RANK_WIDTH, RANK_HEIGHT, TFT_BLACK);
display.setTextColor(player1.color);
display.drawString(String(player1.rank), player1.positionX + RANK_X, player1.positionY + RANK_Y);
display.setTextColor(player2.color);
display.drawString(String(player2.rank), player2.positionX + RANK_X, player2.positionY + RANK_Y);*/
//}

void Display::ShowWinner(OnePlayer player, bool reverseColor)
{
    uint16_t color = TFT_YELLOW;
    if (reverseColor)
        color = TFT_DARKCYAN;
    display.fillCircle(player.positionX + RANK_WINNER_X, player.positionY + RANK_WINNER_Y, RANK_WINNER_RADIUS, color);
    ShowRank(player, false);
}
