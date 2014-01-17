#ifndef _TETROMINO_
#define _TETROMINO_

class Tetromino
{
public:

	float red;
	float green;
	float blue;
	int type;
	int getBlockType(int tetroType, int rotationType, int sizeX, int sizeY);
	int getXInitialPosition(int tetroType, int rotationType);
	int getYInitialPosition(int tetroType, int rotationType);
	void setColor(int t);

};

#endif