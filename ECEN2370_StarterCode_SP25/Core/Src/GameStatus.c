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
static int directions[4][2][2] = {{{0, 1}, {0, -1}}, {{1, 0}, {-1, 0}}, {{-1, -1}, {1, 1}}, {{-1, 1}, {1, -1}}};
static uint8_t gameModeVariable;

void boardDisplay(void) {
	LCD_Clear(0, LCD_COLOR_WHITE);
//	for (uint8_t x=BOARD_LEFT; x< LCD_PIXEL_WIDTH; x+=30) {
//		LCD_Draw_Vertical_Line(x, BOARD_TOP, BOARD_HEIGHT, LCD_COLOR_BLACK);
//	}

	LCD_Draw_Rectangle_Fill(BOARD_LEFT, BOARD_TOP, 215, BOARD_HEIGHT, LCD_COLOR_BLUE);

	drawCoins();
}

void drawCoins(void) {
//	initBoard();
	Board board = getCurrentBoard();

//	uint8_t **board = getCurrentBoard();

	for(uint8_t i = 0; i < NUM_ROWS; i++) {
		for(uint8_t j = 0; j < NUM_COLS; j++) {
			uint16_t x = BOARD_LEFT + (COLUMN_WIDTH/2) + (j*COLUMN_WIDTH);
			uint16_t y = BOARD_TOP + COIN_RADIUS + (i*(COIN_DIAMETER + GAP_BETWEEN_COINS));
			if (board.data[i][j] == PLAYER_1) {
				LCD_Draw_Circle_Fill(x, y, COIN_RADIUS, LCD_COLOR_RED);
			}
			else if (board.data[i][j] == PLAYER_2) {
				LCD_Draw_Circle_Fill(x, y, COIN_RADIUS, LCD_COLOR_YELLOW);
			}
			else {
				LCD_Draw_Circle_Fill(x, y, COIN_RADIUS, LCD_COLOR_WHITE);
			}
		}
	}
}

// maybe move to application code??
uint8_t leftOrRight(void) {
	while(winner == EMPTY_SPACE && totalCoinNumber != TOTAL_SPOTS) {
		// just return if it's the computer's turn now
		if (currentPlayer == PLAYER_2 && gameModeVariable == ONE_PLAYER_MODE) {
			return EMPTY_SPACE;
		}

		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			if (StaticTouchData.x < SCREEN_MIDDLE) {
//				LCD_Clear(0,LCD_COLOR_BLUE);
				return RIGHT_SIDE;
			}
			else {
//				LCD_Clear(0,LCD_COLOR_RED);
				return LEFT_SIDE;
			}
		}
	}
}

void drawFloatingCoin(uint16_t col, uint8_t player) {
	boardDisplay();

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

bool addCoin(uint8_t col, uint8_t player) {
	uint8_t availRow = nextAvailSpots[col];
	if (availRow == 255) {
		return false;
	}

	board.data[availRow][col] = player;

	nextAvailSpots[col] -= 1;
	return true;
}

void changeFloatingCoin(uint8_t direction) {
	if (direction == LEFT_SIDE ) {
		if (floatingCoinCol > 0) {
			floatingCoinCol -= 1;
			HAL_Delay(150);
		}
	}
	else if (direction == RIGHT_SIDE){
		if (floatingCoinCol < NUM_COLS-1) {
			floatingCoinCol += 1;
			HAL_Delay(150);
		}
	}
}

void takeTurn() {
	// don't allow user to make a move when it's the computer's turn
	if (currentPlayer == PLAYER_2 && gameModeVariable == ONE_PLAYER_MODE) {
		return;
	}

	bool validPosition = addCoin(floatingCoinCol, currentPlayer);
	if (validPosition == true) {
		uint8_t posX = nextAvailSpots[floatingCoinCol] + 1;
		uint8_t posY = floatingCoinCol;

		winner = evaluateForWinner(posX, posY);

		if (winner != EMPTY_SPACE) {
			return;
		}

		if (currentPlayer == PLAYER_1) {
			currentPlayer = PLAYER_2;
		}
		else {
			currentPlayer = PLAYER_1;
		}
		totalCoinNumber += 1;
		boardDisplay();
		drawFloatingCoin(floatingCoinCol, currentPlayer);
	}
	// add condition for if you're at 42 coins after adding this coin
}

// returns 0 if no winner, 1 for p1, 2 for p2
uint8_t evaluateForWinner(uint8_t x, uint8_t y) {
	uint8_t count = evalMove(x, y, currentPlayer);
	if (count >= 4) {
		return currentPlayer;
	}
	return EMPTY_SPACE;
}

// returns the max number of coins in a row the player would have with a certain move
uint8_t evalMove(uint8_t x, uint8_t y, uint8_t player) {
	uint8_t maxPoints =  0;
	for (int i = 0; i < 4; i++) {
		uint8_t count = 1;

		count += checkDirection(x, y, directions[i][0][0], directions[i][0][1], currentPlayer);
		count += checkDirection(x, y, directions[i][1][0], directions[i][1][1], currentPlayer);

		if (count > maxPoints) {
			maxPoints = count;
		}
	}
	return maxPoints;
}

uint8_t checkDirection(uint8_t x, uint8_t y, uint8_t m, uint8_t n, uint8_t player) {
	uint8_t currCount = 0;
	uint8_t currX = x + m;
	uint8_t currY = y + n;

	while (0 <= currX && currX < NUM_ROWS && 0 <= currY && currY < NUM_COLS && board.data[currX][currY] == player) {
		currCount += 1;
		currX += m;
		currY += n;
	}

	return currCount;
}

// returns the best column to place coin
uint8_t AITurn(void) {
	uint8_t colPointCount[NUM_COLS];
	uint8_t maxPoints =  0;
	uint8_t maxPointCol = 0;

	// check for offensive move or best col for scoring 4 in a row
	for (uint8_t j = 0; j < NUM_COLS; j++) {
		uint8_t posX = nextAvailSpots[j];
		uint8_t posY = j;

		// if the column is full, then skip
		if (posX > 10) {
			continue;
		}

		for (int i = 0; i < 4; i++) {
			uint8_t count = 1;

			count += checkDirection(posX, posY, directions[i][0][0], directions[i][0][1], PLAYER_2);
			count += checkDirection(posX, posY, directions[i][1][0], directions[i][1][1], PLAYER_2);

			// if there's already a winning move, take it
			if (count >= 4) {
				addCoin(j, PLAYER_2);
				winner = PLAYER_2;
				return j;
			}

			colPointCount[j] = count;
			if (count > maxPoints) {
				maxPoints = count;
				maxPointCol = j;
			}
		}
	}

	// check for defensive moves
	for (uint8_t j = 0; j < NUM_COLS; j++) {
		uint8_t posX = nextAvailSpots[j];
		uint8_t posY = j;

		// if the column is full, then skip
		if (posX > 10) {
			continue;
		}

		for (int i = 0; i < 4; i++) {
			uint8_t count = 1;

			count += checkDirection(posX, posY, directions[i][0][0], directions[i][0][1], PLAYER_1);
			count += checkDirection(posX, posY, directions[i][1][0], directions[i][1][1], PLAYER_1);

			// if there's a winning move for p1, steal the spot
			if (count >= 4) {
				addCoin(j, PLAYER_2);
				return j;
			}
		}
	}

	// if neither of the two, just make the move w the max points
	addCoin(maxPointCol, PLAYER_2);
	return maxPointCol;
}

// maybe move to application code - returns the winner of the game
// change to take an input of game mode
uint8_t gamePlay(uint8_t gameMode) {
	gameModeVariable = gameMode;
	initBoard();
	currentPlayer = PLAYER_1;
	boardDisplay();
	if (gameMode == TWO_PLAYER_MODE) {
		while ((totalCoinNumber != TOTAL_SPOTS) && (winner == EMPTY_SPACE)) {
			drawFloatingCoin(floatingCoinCol, currentPlayer);
			uint8_t direction = leftOrRight();
			changeFloatingCoin(direction);
		}
		LCD_Clear(0, LCD_COLOR_MAGENTA);
		HAL_Delay(5000);
	}
	else {
		while ((totalCoinNumber != TOTAL_SPOTS) && (winner == EMPTY_SPACE)) {
			if (currentPlayer == PLAYER_1) {
				drawFloatingCoin(floatingCoinCol, currentPlayer);
				uint8_t direction = leftOrRight();
				changeFloatingCoin(direction);
			}
			else {
				uint8_t AICol = AITurn();
				drawFloatingCoin(AICol, currentPlayer);
				HAL_Delay(1000);
				boardDisplay();
				currentPlayer = PLAYER_1;
			}
		}
		LCD_Clear(0, LCD_COLOR_MAGENTA);
		HAL_Delay(5000);
	}

	return winner;
}

