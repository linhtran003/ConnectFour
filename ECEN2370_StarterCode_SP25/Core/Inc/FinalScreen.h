/*
 * FinalScreen.h
 *
 *  Created on: Apr 29, 2025
 *      Author: linhtran
 */

#ifndef INC_FINALSCREEN_H_
#define INC_FINALSCREEN_H_

#include "OpeningScreen.h"
#include <string.h>

#define HOME_SELECTED 1
#define REPLAY_SELECTED 0

#define PLAYER_TEXT_X 30
#define P1_Y 10
#define P2_Y 40
#define TIME_TEXT_Y 80
#define TIME_NUM_Y 110

#define TOTAL_TEXT_LENGTH 25
#define RED_WIN_TEXT_LENGTH 10
#define YELLOW_WIN_TEXT_LENGTH 13
#define TIME_TEXT_LENGTH 11
#define REPLAY_TEXT_LENGTH 11
#define HOME_TEXT_LENGTH 4

#define BUTTON3_Y 150
#define BUTTON4_Y (BUTTON3_Y + BUTTON_HEIGHT + 25)

#define REPLAY_TEXT_X (BUTTON_STARTING_TEXT_X - 5)
#define HOME_TEXT_X (BUTTON_STARTING_TEXT_X + 45)

#define HOME_TEXT_Y (BUTTON4_Y + (BUTTON_HEIGHT/2) - 10)
#define REPLAY_TEXT_Y (BUTTON3_Y + (BUTTON_HEIGHT/2) - 10)

void displayGameStats(uint8_t p1Wins, uint8_t p2Wins, int gameTime);
void drawReplayButton(void);
void drawGameModeButton(void);
uint8_t selectButton(void);
uint8_t displayFinalScreen(uint8_t p1Wins, uint8_t p2Wins, uint32_t gameTime);

#endif /* INC_FINALSCREEN_H_ */
