#ifndef _GAME_
#define _GAME_
  
#include "Board.h"
#include "Tetromino.h"
#include <time.h> 

#include <gl\glew.h>
#include <gl\wglew.h>
#include <gl\glut.h>
#include <gl\GL.h>



enum color {BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, COLOR_MAX}; // Colors

class Game
{
public:
 
    Game(Board *pBoard, Tetromino *tetrominos, int pScreenHeight);
 
    void drawScene ();
    void createNewPiece ();
 
	//4 variables that define a piece
    int mPosX, mPosY;               // Position of the piece that is falling down
    int tetroType, mRotation;          // Kind and rotation the piece that is falling down
	

private:
 
    int mScreenHeight;              // Screen height in pixels
    int mNextPosX, mNextPosY;       // Position of the next piece
    int nextTetroType, mNextRotation;  // Kind and rotation of the next piece
 
    Board *mBoard;
    Tetromino *tetromino;
 
    int getRand (int pA, int pB);
    void initGame();
    void drawPiece (int pX, int pY, int pTetroType, int pRotation);
    void drawBoard ();
	GLuint texture;
	GLuint LoadTexture(const char * filename);

};
#endif 