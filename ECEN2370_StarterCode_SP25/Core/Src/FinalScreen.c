/*
 * FinalScreen.c
 *
 *  Created on: Apr 29, 2025
 *      Author: linhtran
 */


#include "FinalScreen.h"

static STMPE811_TouchData StaticTouchData;

void displayGameStats(uint8_t p1Wins, uint8_t p2Wins, int gameTime) {
	LCD_Clear(0, LCD_COLOR_WHITE);

	uint8_t p1TextLength = 0;
	uint8_t p2TextLength = 0;
	uint8_t p1TotalTextLength = 0;
	uint8_t p2TotalTextLength = 0;
	uint8_t timeLength = 0;

	if (p1Wins/10 == 0) {
		p1TextLength = 1;
	}
	else {
		p1TextLength = 2;
	}

	if (p2Wins/10 == 0) {
		p2TextLength = 1;
	}
	else {
		p2TextLength = 2;
	}

	char p1WinText[4];
	char p2WinText[4];

	sprintf(p1WinText, "%d", p1Wins);
	sprintf(p2WinText, "%d", p2Wins);

	p1TotalTextLength = RED_WIN_TEXT_LENGTH + p1TextLength;
	p2TotalTextLength = YELLOW_WIN_TEXT_LENGTH + p2TextLength;

	char text1[RED_WIN_TEXT_LENGTH + 3] = "Red Wins: ";
	char text2[YELLOW_WIN_TEXT_LENGTH + 3] = "Yellow Wins: ";

	strcat(text1, p1WinText);
	strcat(text2, p2WinText);

	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);

	for (uint8_t i = 0; i < p1TotalTextLength; i++) {
		LCD_DisplayChar(PLAYER_TEXT_X + (i*11), P1_Y, text1[i]);
	}
	for (uint8_t i = 0; i < p2TotalTextLength; i++) {
		LCD_DisplayChar(PLAYER_TEXT_X + (i*11), P2_Y, text2[i]);
	}

	char timeText[TIME_TEXT_LENGTH];

	sprintf(timeText, "%d", gameTime);

	char timeString[TIME_TEXT_LENGTH + 20] = "Game Time: ";

	strcat(timeText, "s");


	do {
		gameTime /= 10;
		timeLength += 1;
	} while (gameTime != 0);

	timeLength += 1;

	for (uint8_t i = 0; i < TIME_TEXT_LENGTH; i++) {
		LCD_DisplayChar(PLAYER_TEXT_X + (i*14), TIME_TEXT_Y, timeString[i]);
	}

	for (uint8_t i = 0; i < timeLength; i++) {
		LCD_DisplayChar(PLAYER_TEXT_X + (i*14), TIME_NUM_Y, timeText[i]);
	}
}

void drawReplayButton(void) {
	LCD_Draw_Rectangle_Fill(BUTTON_X, BUTTON3_Y, BUTTON_WIDTH, BUTTON_HEIGHT, LCD_COLOR_BLUE2);

	char text[REPLAY_TEXT_LENGTH] = "REPLAY GAME";
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);

	for (uint8_t i = 0; i < REPLAY_TEXT_LENGTH; i++) {
		LCD_DisplayChar(REPLAY_TEXT_X + (i*15), REPLAY_TEXT_Y, text[i]);
	}
}

void drawGameModeButton(void) {
	LCD_Draw_Rectangle_Fill(BUTTON_X, BUTTON4_Y, BUTTON_WIDTH, BUTTON_HEIGHT, LCD_COLOR_MAGENTA);

	char text[HOME_TEXT_LENGTH] = "HOME";
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);

	for (uint8_t i = 0; i < HOME_TEXT_LENGTH; i++) {
		LCD_DisplayChar(HOME_TEXT_X + (i*15), HOME_TEXT_Y, text[i]);
	}
}

uint8_t selectButton(void) {
	while(1) {
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			if (BUTTON_X < StaticTouchData.x && StaticTouchData.x < (BUTTON_X + BUTTON_WIDTH)
					&& BUTTON3_Y < StaticTouchData.y && StaticTouchData.y < (BUTTON3_Y + BUTTON_HEIGHT)) {
				return REPLAY_SELECTED;
			}
			else if (BUTTON_X < StaticTouchData.x && StaticTouchData.x < (BUTTON_X + BUTTON_WIDTH)
					&& BUTTON4_Y < StaticTouchData.y && StaticTouchData.y < (BUTTON4_Y + BUTTON_HEIGHT)) {
				return HOME_SELECTED;
			}
		}
	}
}

uint8_t displayFinalScreen(uint8_t p1Wins, uint8_t p2Wins, uint32_t gameTime) {
	displayGameStats(p1Wins, p2Wins, gameTime);
	drawReplayButton();
	drawGameModeButton();
	return selectButton();
}
