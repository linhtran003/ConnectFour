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
uint8_t evaluateForWinner(uint8_t x, uint8_t y);
uint8_t checkDirection(uint8_t x, uint8_t y, uint8_t i, uint8_t j);
uint8_t gamePlay(void); // function for overall gameplay - returns the winner

#endif /* INC_GAMESTATUS_H_ */
