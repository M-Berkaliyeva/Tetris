#ifndef _BOARD_
#define _BOARD_
  
#include "Tetromino.h"
  
#define BOARD_LINE_WIDTH 6          // Width of each of the two lines that delimit the board
#define BLOCK_SIZE 30               // Width and Height of each block of a tetromino
#define BOARD_POSITION 220          // Center position of the board from the left of the screen
#define BOARD_WIDTH 10              // Board width in blocks
#define BOARD_HEIGHT 20             // Board height in blocks
#define MIN_VERTICAL_MARGIN 20      // Minimum vertical margin for the board limit     
#define MIN_HORIZONTAL_MARGIN 20    // Minimum horizontal margin for the board limit
#define PIECE_BLOCKS 5// Number of horizontal and vertical blocks of a matrix piece

class Board
{
public: 

	Board(Tetromino *tetros, int screenHeight);

	int getXPosInPixels(int pPos);
	int getYPosInPixels(int pPos);
	bool isFreeBlock(int pX, int pY);
	bool isPossibleMovement(int pX, int pY, int tetroType, int rotationtype);
	void storeTetromino(int pX, int pY, int tetroType, int rotationType);
	void deletePossibleLines();
	bool isGameOver();
	void initBoard();
	
	bool gameOver;

	int deletedLineCount;
	int points;
	int score;
	int level;
	int timer;

private:

	enum {POS_FREE, POS_FILLED};
	int board[BOARD_WIDTH][BOARD_HEIGHT];
	Tetromino *tetromino;
	int screenHeight;

	
	void deleteLine(int pY);
};

#endif