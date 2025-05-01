/*
 * GameStatus.c
 *
 *  Created on: Apr 22, 2025
 *      Author: linhtran
 */
#include <GameStatus.h>

// (0, 0) IS AT THE TOP LEFT CORNER OF THE BOARD
static Board board;
static uint8_t nextAvailSpots[NUM_COLS];
static uint16_t totalCoinNumber;
static uint8_t winner;
static uint8_t currentPlayer;
static STMPE811_TouchData StaticTouchData;
static uint8_t floatingCoinCol = 3;
static int directions[4][2][2] = {{{0, 1}, {0, -1}}, {{1, 0}, {-1, 0}}, {{-1, -1}, {1, 1}}, {{-1, 1}, {1, -1}}};
static uint8_t gameModeVariable;

void boardDisplay(void) {
	LCD_Clear(0, LCD_COLOR_WHITE);

	LCD_Draw_Rectangle_Fill(BOARD_LEFT, BOARD_TOP, 215, BOARD_HEIGHT, LCD_COLOR_BLUE);

	drawCoins();
}

void drawCoins(void) {
	Board board = getCurrentBoard();

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
				return RIGHT_SIDE;
			}
			else {
				return LEFT_SIDE;
			}
		}
	}
	return EMPTY_SPACE;
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

void takeTurn(void) {
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
}

// returns 0 if no winner, 1 for p1, 2 for p2
uint8_t evaluateForWinner(uint8_t x, uint8_t y) {
	uint8_t count = evalMove(x, y, currentPlayer, board);
	if (count >= 4) {
		return currentPlayer;
	}
	return EMPTY_SPACE;
}

// returns the max number of coins in a row the player would have with a certain move
uint8_t evalMove(uint8_t x, uint8_t y, uint8_t player, Board b) {
	uint8_t maxPoints =  0;
	for (int i = 0; i < 4; i++) {
		uint8_t count = 1;

		count += checkDirection(x, y, directions[i][0][0], directions[i][0][1], player, b);
		count += checkDirection(x, y, directions[i][1][0], directions[i][1][1], player, b);

		if (count > maxPoints) {
			maxPoints = count;
		}
	}
	return maxPoints;
}

// returns true if the next player's move would be a win for them, false otherwise
bool evalPlayer1NextMove(uint8_t y, uint8_t player, Board potentialBoard) {
	// do not check position of out of range - prevents segfault
	if (nextAvailSpots[y] == 0 || nextAvailSpots[y] > 10) {
		return false;
	}

	uint8_t count = 0;

	count = evalMove(nextAvailSpots[y] - 1, y, player, potentialBoard);

	if (count >= 4) {
		return true;
	}
	return false;
}

uint8_t checkDirection(uint8_t x, uint8_t y, uint8_t m, uint8_t n, uint8_t player, Board b) {
	uint8_t currCount = 0;
	uint8_t currX = x + m;
	uint8_t currY = y + n;

	while (0 <= currX && currX < NUM_ROWS && 0 <= currY && currY < NUM_COLS && b.data[currX][currY] == player) {
		currCount += 1;
		currX += m;
		currY += n;
	}

	return currCount;
}

// returns the column that would result in most "points" for AI
uint8_t findMaxPointColumn(uint8_t colPointCount[]) {
	uint8_t maxPoints =  0;
	uint8_t maxPointCol = 0;
	bool colChanged = false;
	for (uint8_t j = 0; j < NUM_COLS; j++) {
		if (nextAvailSpots[j] > 10) {
			if (colChanged == false) {
				maxPointCol += 1;
				colChanged = true;
			}
			continue;
		}

		if (colPointCount[j] > maxPoints) {
			maxPoints = colPointCount[j];
			maxPointCol = j;
		}
	}
	return maxPointCol;
}

// returns the best column to place coin
uint8_t AITurn(void) {
	uint8_t colPointCount[NUM_COLS];
	uint8_t maxPointCol = 0;
	uint8_t count = 0;
	bool isAssistMove = true;
	bool validMove = false;

	// check for offensive move or best col for scoring 4 in a row
	for (uint8_t j = 0; j < NUM_COLS; j++) {
		uint8_t posX = nextAvailSpots[j];
		uint8_t posY = j;
		count = 0;

		// if the column is full, then skip
		if (posX > 10) {
			continue;
		}

		count = evalMove(posX, posY, currentPlayer, board);

		// if there's already a winning move, take it
		if (count >= 4) {
			addCoin(j, PLAYER_2);
			winner = PLAYER_2;
			return j;
		}

		colPointCount[j] = count;
	}

	// check for defensive moves
	for (uint8_t j = 0; j < NUM_COLS; j++) {
		uint8_t posX = nextAvailSpots[j];
		uint8_t posY = j;
		count = 0;

		// if the column is full, then skip
		if (posX > 10) {
			continue;
		}

		count = evalMove(posX, posY, PLAYER_1, board);
		if (count >= 4) {
			addCoin(j, PLAYER_2);
			return j;
		}
	}

	// lastly, if the maxPoint move assists in a win for the other player, then don't make that move - make next maxPoint move
	uint8_t k = 0;
	while (isAssistMove == true && k < NUM_COLS) {
		maxPointCol = findMaxPointColumn(colPointCount);

		uint8_t posX = nextAvailSpots[maxPointCol];
		uint8_t posY = maxPointCol;

		if (posX > 10) {
			k += 1;
			continue;
		}

		Board potentialBoard;

		for (uint8_t m = 0; m < NUM_ROWS; m++) {
			for (uint8_t n = 0; n < NUM_COLS; n++) {
				potentialBoard.data[m][n] = board.data[m][n];
			}
		}

		potentialBoard.data[posX][posY] = currentPlayer;
		isAssistMove = evalPlayer1NextMove(posY, PLAYER_1, potentialBoard);

		if (isAssistMove == true) {
			colPointCount[maxPointCol] = 0;
		}

		k += 1;
	}

	// if neither of the two, just make the move w the max points
	validMove = addCoin(maxPointCol, PLAYER_2);
	if (validMove == false) {
		for (uint8_t m = 0; m < NUM_COLS; m++) {
			if (nextAvailSpots[m] < NUM_ROWS) {
				addCoin(m, PLAYER_2);
				return m;
			}
		}
	}

	return maxPointCol;
}

// returns the winner of the game
uint8_t gamePlay(uint8_t gameMode) {
	gameModeVariable = gameMode;
	initBoard();
	currentPlayer = PLAYER_1;
	boardDisplay();
	totalCoinNumber = 0;
	winner = EMPTY_SPACE;
	if (gameMode == TWO_PLAYER_MODE) {
		while ((totalCoinNumber != TOTAL_SPOTS) && (winner == EMPTY_SPACE)) {
			drawFloatingCoin(floatingCoinCol, currentPlayer);
			uint8_t direction = leftOrRight();
			changeFloatingCoin(direction);
		}

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
	}

	return winner;
}

