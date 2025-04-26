/*
 * GameStatus.h
 *
 *  Created on: Apr 22, 2025
 *      Author: linhtran
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <LCD_Driver.h>

#ifndef INC_GAMESTATUS_H_
#define INC_GAMESTATUS_H_

#define NUM_ROWS 0x6
#define NUM_COLS 0x7
#define EMPTY_SPACE 0
#define PLAYER_1 1
#define PLAYER_2 2
#define TOTAL_SPOTS 42

#define ONE_PLAYER_MODE 1
#define TWO_PLAYER_MODE 2

typedef struct {
    uint8_t data[NUM_ROWS][NUM_COLS];
} Board;

//typedef struct {
//    uint8_t data[8];
//} Neighbors;

void boardDisplay(void);
void drawCoins(void);
uint8_t leftOrRight(void);
void drawFloatingCoin(uint16_t col, uint8_t player);



Board getCurrentBoard(void);
void initBoard(void);
void emptyBoard(void);
bool addCoin(uint8_t col, uint8_t player);
void setButtonPressed(bool pressed);
//void player2Turn(void);
uint8_t AITurn(void);
uint8_t evaluateForWinner(uint8_t x, uint8_t y);
uint8_t checkDirection(uint8_t x, uint8_t y, uint8_t m, uint8_t n, uint8_t player);
uint8_t gamePlay(uint8_t gameMode); // function for overall gameplay - returns the winner

#endif /* INC_GAMESTATUS_H_ */
