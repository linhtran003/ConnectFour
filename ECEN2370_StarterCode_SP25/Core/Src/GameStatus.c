/*
 * GameStatus.c
 *
 *  Created on: Apr 22, 2025
 *      Author: linhtran
 */
#include <GameStatus.h>

// (0, 0) IS AT THE TOP LEFT CORNER OF THE BOARD!!!
static Board board;
//static Neighbors neighbors;
static uint8_t nextAvailSpots[NUM_COLS];
static uint16_t totalCoinNumber;
static uint8_t winner;
static uint8_t currentPlayer;
static bool buttonPressed = false;
static STMPE811_TouchData StaticTouchData;
static uint8_t floatingCoinCol = 3;

void boardDisplay(void) {

	LCD_Clear(0, LCD_COLOR_WHITE);
	for (uint8_t x=BOARD_LEFT; x< LCD_PIXEL_WIDTH; x+=30) {
		LCD_Draw_Vertical_Line(x, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_BLACK);
	}

	drawCoins();
}

void drawCoins(void) {
	initBoard();
	Board board = getCurrentBoard();

//	uint8_t **board = getCurrentBoard();

	for(uint8_t i = 0; i < NUM_ROWS; i++) {
		for(uint8_t j = 0; j < NUM_COLS; j++) {
			if (board.data[i][j] != EMPTY_SPACE) {
				uint16_t x = BOARD_LEFT + (COLUMN_WIDTH/2) + (j*COLUMN_WIDTH);
				uint16_t y = BOARD_TOP + COIN_RADIUS + (i*(COIN_DIAMETER + GAP_BETWEEN_COINS));
				if (board.data[i][j] == PLAYER_1) {
					LCD_Draw_Circle_Fill(x, y, COIN_RADIUS, LCD_COLOR_RED);
				}
				else if (board.data[i][j] == PLAYER_2) {
					LCD_Draw_Circle_Fill(x, y, COIN_RADIUS, LCD_COLOR_YELLOW);
				}
			}
		}
	}
}

// maybe move to application code??
uint8_t leftOrRight(void) {
	while(1) {
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			if (0 < StaticTouchData.x && StaticTouchData.x < SCREEN_MIDDLE) {
//				LCD_Clear(0,LCD_COLOR_BLUE);
				return LEFT_SIDE;
			}
			else {
//				LCD_Clear(0,LCD_COLOR_RED);
				return RIGHT_SIDE;
			}
		}
	}
}

void drawFloatingCoin(uint16_t col, uint8_t player) {
	uint16_t x = BOARD_LEFT + (COLUMN_WIDTH/2) + (col*COLUMN_WIDTH);
	uint16_t y = BOARD_TOP - 50;

	if (player == PLAYER_1) {
		LCD_Draw_Circle_Fill(x, y, COIN_RADIUS, LCD_COLOR_RED);
	}
	else {
		LCD_Draw_Circle_Fill(x, y, COIN_RADIUS, LCD_COLOR_YELLOW);
	}
}

///////////////////////////////////////////////////////////////// BEGIN GAME LOGIC

void initBoard(void) {
	emptyBoard();

	// filling out board for
//	for (uint8_t k = 0; k < NUM_ROWS; k++) {
//		for (uint8_t j = 0; j < NUM_COLS; j++) {
//			if (k%2 == 0) {
//				board.data[k][j] = PLAYER_1;
//			}
//			else {
//				board.data[k][j] = PLAYER_2;
//			}
//		}
//	}

	for (uint8_t i = 0; i < NUM_COLS; i++) {
		nextAvailSpots[i] = NUM_ROWS-1;
	}
}

void emptyBoard(void) {
	for (uint8_t i = 0; i < NUM_ROWS; i++) {
		for (uint8_t j = 0; j < NUM_COLS; j++) {
			board.data[i][j] = EMPTY_SPACE;
		}
	}
}

Board getCurrentBoard(void) {
	return board;
}

void addCoin(uint8_t col, uint8_t player) {
	uint8_t availRow = nextAvailSpots[col];
	if (availRow < 0) {
		return;
	}

	board.data[availRow][col] = player;

	nextAvailSpots[col] -= 1;
}

void setButtonPressed(bool pressed) {
	buttonPressed = pressed;
}

// maybe move to application code
uint8_t gamePlay(void) {
	initBoard();
	currentPlayer = PLAYER_1;
	while ((totalCoinNumber != TOTAL_SPOTS) && (winner == EMPTY_SPACE)) {
		boardDisplay();
		if (currentPlayer == PLAYER_1) {
			while(buttonPressed == false) {
				drawFloatingCoin(floatingCoinCol, PLAYER_1);
				uint8_t direction = leftOrRight();
				if (direction == LEFT_SIDE ) {
					if (floatingCoinCol > 0) {
						floatingCoinCol -= 1;
					}
				}
				else {
					if (floatingCoinCol < NUM_COLS-1) {
						floatingCoinCol += 1;
					}
				}
			}

			addCoin(floatingCoinCol, PLAYER_1);

			buttonPressed = false;
//			currentPlayer = PLAYER_2;
		}
		else {

			currentPlayer = PLAYER_1;
		}
	}
}

//void hasValidNeighbor(uint8_t x, uint8_t y) {
//
//}
//
//void determineWinner(uint8_t x, uint8_t y) {
//	uint8_t winner = EMPTY_SPACE;
//	uint8_t count = EMPTY_SPACE;
//
//}
