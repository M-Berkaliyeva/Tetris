#include "stdafx.h"
#include "Game.h"
#include <stdlib.h> 
#include <iostream>

Game::Game(Board *pBoard, Tetromino *pPieces, int pScreenHeight) 
{
	mScreenHeight = pScreenHeight;

	// Get the pointer to the Board and Tetromino classes
	mBoard = pBoard;
	tetromino = pPieces;

	// Game initialization
	initGame ();
}

/*
returns a random number between two numbers
*/
int Game::getRand(int pA, int pB)
{
	return rand() % (pB - pA +1) +pA;
}

/*
new game is initialied by creating two pieces: current and next piece randomly and then sets their positions
*/
void Game::initGame()
{
	//init random numbers
	srand(GetTickCount());

	//1st piece
	tetroType = getRand(0, 6);
	mRotation = getRand(0, 3);
	mPosX = (BOARD_WIDTH/2) + tetromino->getXInitialPosition(tetroType, mRotation);
	mPosY = tetromino->getYInitialPosition(tetroType, mRotation);

	//next piece
	nextTetroType = getRand(0, 6);
	mNextRotation = getRand(0, 3);
	mNextPosX = (BOARD_WIDTH) + 2;
	mNextPosY = 0;
}

/*
create a random piece
*/
void Game::createNewPiece()
{
	//new piece
	tetroType = nextTetroType;
	mRotation = mNextRotation;
	mPosX = (BOARD_WIDTH/2) + tetromino->getXInitialPosition(tetroType, mRotation);
	mPosY = tetromino->getYInitialPosition(tetroType, mRotation);
	
	//next piece
	nextTetroType = getRand(0, 6);
	mNextRotation = getRand(0, 3);
}

/*
Metod iterates through the piece matrix and draws each block of the piece
*/
void Game::drawPiece(int pX, int pY, int pTetroType, int pRotation)
{
	color mColor;
 
    // Obtain the position in pixel in the screen of the block we want to draw
    int mPixelsX = mBoard->getXPosInPixels (pX);
    int mPixelsY = mBoard->getYPosInPixels (pY);
 
    // Travel the matrix of blocks of the piece and draw the blocks that are filled
    for (int i = 0; i < PIECE_BLOCKS; i++)
    {
        for (int j = 0; j < PIECE_BLOCKS; j++)
        {
			/*tetromino->setColor(tetroType);
				glColor3f(GLfloat(tetromino->red), GLfloat(tetromino->green), GLfloat(tetromino->blue));*/
             //Get the type of the block and draw it with the correct color
            switch (tetromino->getBlockType (pTetroType, pRotation, j, i))
            {
                case 1: mColor = GREEN; glColor3f(0.0f, 1.0f, 0.0f); break;  // For each block of the piece except the pivot
                case 2: mColor = BLUE; glColor3f(0.0f, 0.0f, 1.0f); break;   // For the pivot
            }
             
            if (tetromino->getBlockType (pTetroType, pRotation, j, i) != 0)
			{
				glRectf(GLfloat(mPixelsX + i * BLOCK_SIZE),
					GLfloat(mPixelsY + j * BLOCK_SIZE),
					GLfloat((mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1),
					GLfloat((mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1));
			}
        }
    }
}

void Game::drawBoard ()
{
	// Calculate the limits of the board in pixels 
    int mX1 = BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) - 1;
    int mX2 = BOARD_POSITION + (BLOCK_SIZE * (BOARD_WIDTH / 2));
    int mY = mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT);

    // Rectangles that delimits the board

	glColor3f(0.0f, 0.0f, 0.0f);
	glRectf(GLfloat(mX1 - BOARD_LINE_WIDTH),
					GLfloat(mY),
					GLfloat(mX1),
					GLfloat(mScreenHeight - 1));

	glRectf(GLfloat(mX2),
					GLfloat(mY),
					GLfloat(mX2 + BOARD_LINE_WIDTH),
					GLfloat(mScreenHeight - 1));

	

    // Drawing the blocks that are already stored in the board
    mX1 += 1;
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {  
            // Check if the block is filled, if so, draw it
            if (!mBoard->isFreeBlock(i, j)) 
			{
				tetromino->setColor(tetroType);
				glColor3f(GLfloat(tetromino->red), GLfloat(tetromino->green), GLfloat(tetromino->blue));
				//glColor3f(0.0f, 1.0f, 1.0f);

				glRectf(GLfloat(mX1 + i * BLOCK_SIZE),
					GLfloat(mY + j * BLOCK_SIZE),
					GLfloat((mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1),
					GLfloat((mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1));

				/*glBegin(GL_QUADS);
					glTexCoord2d(0.0f, 0.0f); 
					glVertex3f(GLfloat(mX1 + i * BLOCK_SIZE), GLfloat(mY + j * BLOCK_SIZE), 0.0f);
					glTexCoord2d(1.0f, 0.0f); 
					glVertex3f(GLfloat((mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1), GLfloat(mY + j * BLOCK_SIZE), 0.0f);
					glTexCoord2d(1.0f, 1.0f); 
					glVertex3f(GLfloat((mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1),GLfloat((mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1), 0.0f);
					glTexCoord2d(0.0f, 1.0f); 
					glVertex3f(GLfloat(mX1 + i * BLOCK_SIZE), GLfloat((mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1), 0.0f);
				glEnd();
*/
				
			}
        }
    }  
	//glDisable(GL_TEXTURE_2D);
}

GLuint Game::LoadTexture( const char * filename )
{

	  GLuint texture;

	  int width, height;

	  unsigned char * data;

	  FILE * file;

	  file = fopen( filename, "rb" );

	  if ( file == NULL ) return 0;
	  width = 30;
	  height = 30;
	  data = (unsigned char *)malloc( width * height * 3 );
	  //int size = fseek(file,);
	  fread( data, width * height * 3, 1, file );
	  fclose( file );

	 for(int i = 0; i < width * height ; ++i)
	{
	   int index = i*3;
	   unsigned char B,R;
	   B = data[index];
	   R = data[index+2];

	   data[index] = R;
	   data[index+2] = B;
	 }
	 
	glEnable(GL_TEXTURE_2D);
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
	//glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free( data );

	return texture;
}

/*
draw all objects of the scene
*/
void Game::drawScene()
{
	//texture = LoadTexture("tile3.bmp");
	drawBoard();
	drawPiece(mPosX, mPosY, tetroType, mRotation);
	drawPiece(mNextPosX, mNextPosY, nextTetroType, mNextRotation);
}