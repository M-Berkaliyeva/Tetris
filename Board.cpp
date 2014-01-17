#include "stdafx.h"
#include "Board.h"
#include <iostream>

Board::Board (Tetromino *tetros, int pScreenHeight)
{
	// Get the screen height
	screenHeight = pScreenHeight;

	// Get the pointer to the tetromino class
	tetromino = tetros;
	
	//Init the board blocks with free positions
	initBoard();
}

/*
Initialize the board blocks with free  positions
*/
void Board::initBoard()
{
	//initialize the time of how fast the game proceeds
	timer = 500;
	//initial number of deleted lines
	deletedLineCount = 0;

	//initial points
	points = 0;

	//initial level
	level = 0;

	//initial score
	score = 0;

	gameOver = false;
	
	for(int col = 0; col < BOARD_WIDTH; col++)
	{
		for(int row = 0; row < BOARD_HEIGHT; row++)
		{
			board[col][row] = POS_FREE;
		}
	}
}
/*
Store a tetromino in the board by filling the block
*/
void Board::storeTetromino(int pX, int pY, int tetroType, int rotationType)
{
	//Store each block of the piece into the board
		for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++)
	{
		for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++)
		{	
			// Store only the blocks of the piece that are not holes
			if (tetromino->getBlockType (tetroType, rotationType, j2, i2) != 0)		
				board[i1][j1] = POS_FILLED;	
		}
	}
}

/*
If the tetromino have achieved the top line -> gameover
*/
bool Board::isGameOver()
{
	//if the upper (first) line has any blocks then game is over
	for(int col = 0; col < BOARD_WIDTH; col++)
	{
		if(board[col][0] == POS_FILLED) 
		{
			gameOver = true;
			return true;
		}
	}

	return false;
}

/*
delete line and move all blocks above one row down
*/

void Board::deleteLine(int pY)
{
	//moves all the upper lines one row down
	for (int j = pY; j > 0; j--)
	{
		for (int i = 0; i < BOARD_WIDTH; i++)
		{
			board[i][j] = board[i][j-1];
		}
	}	
	deletedLineCount++;
	//if ten lines have been deleted go to next level
	if((deletedLineCount % 10) == 0)
	{
		level++;
	}
	if(level>0)
	{
		timer = 500 / level;
	}
}

/*
Remove line that is horizontally filled with blocks and move all lines above it one row down 
*/
void Board::deletePossibleLines()
{
	int countDeletedLines = 0;//number of simultaneously deeted lines
	bool deleted = false;
	for (int j = 0; j < BOARD_HEIGHT; j++)
	{
		int i = 0;
		while (i < BOARD_WIDTH)
		{
			if (board[i][j] != POS_FILLED) break;
			i++;
		}

		if (i == BOARD_WIDTH) 
		{
			deleteLine (j);	
			countDeletedLines++;
			deleted = true;
		}
	}


	//more lines you delete simultaneously, more points you get
	if(countDeletedLines == 1)
	{
		points = 40;
	}
	else if(countDeletedLines == 2)
	{
		points = 100;
	}
	else if(countDeletedLines == 3)
	{
		points = 300;
	}
	else if(countDeletedLines >= 4)
	{
		points = 1200;
	}

	//calculate the final score: 
	if(deleted)
	{		
		score += points *(level + 1);
	}
	
	
}

/*
checks if the current block is free
*/
bool Board::isFreeBlock(int pX, int pY)
{
	if(board[pX][pY] == POS_FREE)
	{
		return true;
	}
	else
	{
		return false;
	}
}



/*
two methods that return the actual position in pixels of a given block
*/

int Board::getXPosInPixels(int pPos)
{
	return ((BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2))) + (pPos * BLOCK_SIZE));
}

int Board::getYPosInPixels(int pPos)
{
	return ((screenHeight  - (BLOCK_SIZE * BOARD_HEIGHT)) + (pPos * BLOCK_SIZE));
}

/*
method that checks if the movement is possible by checking the collision with other blocks and collision with the board limits
*/

bool Board::isPossibleMovement(int pX, int pY, int tetroType, int rotationType)
{
	for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++)
	{
		for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++)
		{	
			// Check if the piece is outside the limits of the board
			if (	i1 < 0 			|| 
				i1 > BOARD_WIDTH  - 1	||
				j1 > BOARD_HEIGHT - 1)
			{
				if (tetromino->getBlockType (tetroType, rotationType, j2, i2) != 0)
					return 0;		
			}

			// Check if the piece have collisioned with a block already stored in the map
			if (j1 >= 0)	
			{
				if ((tetromino->getBlockType (tetroType, rotationType, j2, i2) != 0) &&
					(!isFreeBlock (i1, j1))	)
					return false;
			}
		}
	}
 
    // No collision
    return true;
}