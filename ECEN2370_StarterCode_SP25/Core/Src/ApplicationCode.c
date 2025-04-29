/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

/* Static variables */


extern void initialise_monitor_handles(void); 

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#endif // COMPILE_TOUCH_FUNCTIONS

static uint8_t continuePlaying = REPLAY_SELECTED;
static uint8_t player1Wins;
static uint8_t player2Wins;
static uint32_t startTime;
static uint32_t totalTime;
static uint8_t winner;
uint8_t gameMode;

void ApplicationInit(void)
{
	initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_WHITE);
    Interrupt_Init(); // for button

    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;
//	StaticTouchData.orientation = STMPE811_Orientation_Landscape_1;

	#endif // COMPILE_TOUCH_FUNCTIONS
}

void LCD_Visual_Demo(void)
{
	visualDemo();
}

void LCD_Board_Display(void) {
	boardDisplay();
}

void EXTI0_IRQHandler(void) {
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);

	takeTurn();

	__HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_0);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void fullGame(void) {
	// before gameplay, put the first screen up
	while(1) {
		gameMode = displayOpeningScreen();
//		HAL_Delay(2000);
		continuePlaying = REPLAY_SELECTED;
		player1Wins = 0;
		player2Wins = 0;

		while (continuePlaying == REPLAY_SELECTED) {
			startTime = HAL_GetTick();
			winner = gamePlay(gameMode);
			totalTime = HAL_GetTick() - startTime;
			if (winner == PLAYER_1) {
				player1Wins += 1;
			}
			else if (winner == PLAYER_2) {
				player2Wins += 1;
			}

			continuePlaying = displayGameStats(player1Wins, player2Wins, totalTime/1000);
			HAL_Delay(200);
		}
	}
}


#if COMPILE_TOUCH_FUNCTIONS == 1
void LCD_Touch_Polling_Demo(void)
{
	LCD_Clear(0,LCD_COLOR_GREEN);
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			/* Touch valid */
			printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
			LCD_Clear(0, LCD_COLOR_RED);
		} else {
			/* Touch not pressed */
			printf("Not Pressed\n\n");
			LCD_Clear(0, LCD_COLOR_GREEN);
		}
	}
}
#endif // COMPILE_TOUCH_FUNCTIONS

