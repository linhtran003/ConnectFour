/*
 * OpeningScreen.c
 *
 *  Created on: Apr 25, 2025
 *      Author: linhtran
 */

#include "OpeningScreen.h"

static STMPE811_TouchData StaticTouchData;

void drawTitle(void) {
	char text[TITLE_LENGTH] = "CONNECT FOUR";
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);

	for (uint8_t i = 0; i < TITLE_LENGTH; i++) {
		LCD_DisplayChar(TITLE_STARTING_X + (i*15), TITLE_Y, text[i]);
	}
}

void drawOnePlayerButton(void) {
	LCD_Draw_Rectangle_Fill(BUTTON_X, BUTTON1_Y, BUTTON_WIDTH, BUTTON_HEIGHT, LCD_COLOR_BLUE2);

	char text[TEXT_LENGTH] = "ONE-PLAYER";
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);

	for (uint8_t i = 0; i < TEXT_LENGTH; i++) {
		LCD_DisplayChar(BUTTON_STARTING_TEXT_X + (i*15), P1_TEXT_Y, text[i]);
	}
}

void drawTwoPlayerButton(void) {
	LCD_Draw_Rectangle_Fill(BUTTON_X, BUTTON2_Y, BUTTON_WIDTH, BUTTON_HEIGHT, LCD_COLOR_MAGENTA);

	char text[TEXT_LENGTH] = "TWO-PLAYER";
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);

	for (uint8_t i = 0; i < TEXT_LENGTH; i++) {
		LCD_DisplayChar(BUTTON_STARTING_TEXT_X + (i*15), P2_TEXT_Y, text[i]);
	}
}

uint8_t selectGameMode(void) {
	while(1) {
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			if (BUTTON_X < StaticTouchData.x && StaticTouchData.x < (BUTTON_X + BUTTON_WIDTH)
					&& BUTTON1_Y < StaticTouchData.y && StaticTouchData.y < (BUTTON1_Y + BUTTON_HEIGHT)) {
				LCD_Clear(0,LCD_COLOR_BLUE);
				return ONE_PLAYER_MODE;
			}
			else if (BUTTON_X < StaticTouchData.x && StaticTouchData.x < (BUTTON_X + BUTTON_WIDTH)
					&& BUTTON2_Y < StaticTouchData.y && StaticTouchData.y < (BUTTON2_Y + BUTTON_HEIGHT)) {
				LCD_Clear(0,LCD_COLOR_MAGENTA);
				return TWO_PLAYER_MODE;
			}
		}
	}
}

uint8_t displayOpeningScreen(void) {
	LCD_Clear(0, LCD_COLOR_WHITE);
	drawTitle();
	drawOnePlayerButton();
	drawTwoPlayerButton();
	return selectGameMode();
}
