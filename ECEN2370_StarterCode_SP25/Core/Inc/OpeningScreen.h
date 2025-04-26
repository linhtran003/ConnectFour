/*
 * OpeningScreen.h
 *
 *  Created on: Apr 25, 2025
 *      Author: linhtran
 */

#ifndef INC_OPENINGSCREEN_H_
#define INC_OPENINGSCREEN_H_

#include "LCD_Driver.h"

#define ONE_PLAYER_MODE 1
#define TWO_PLAYER_MODE 2

#define TITLE_LENGTH 12
#define TITLE_Y 50
#define TITLE_STARTING_X 35

#define BUTTON_WIDTH 170
#define BUTTON_HEIGHT 70
#define BUTTON_X (120-(BUTTON_WIDTH/2))
#define BUTTON1_Y 130
#define BUTTON2_Y (BUTTON1_Y + BUTTON_HEIGHT + 30)

#define BUTTON_STARTING_TEXT_X (BUTTON_X + 10)
#define P1_TEXT_Y (BUTTON1_Y + (BUTTON_HEIGHT/2) - 10)
#define P2_TEXT_Y (BUTTON2_Y + (BUTTON_HEIGHT/2) - 10)
#define TEXT_LENGTH 10

uint8_t selectGameMode(void);
void drawOnePlayerButton(void);
void drawTwoPlayerButton(void);
void drawTitle(void);
uint8_t displayOpeningScreen(void);


#endif /* INC_OPENINGSCREEN_H_ */
