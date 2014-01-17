// Tetris.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

#include <time.h>
#include <GL/glut.h>
#include "Game.h"

//shader ID
GLuint Program;
//attribute ID
GLint Attrib_vertex;
//color uniform ID
GLint Unif_color;
//vertex buffer object ID
GLuint VBO;
struct vertex
{
	GLfloat x;
	GLfloat y;
};

std::string key;

int mScreenHeight;
int screenWidth;
GLuint texture = 0;
// Tetromino
Tetromino tetros;
 
// Board
Board mBoard (&tetros, 880);
 
// Game
Game mGame (&mBoard, &tetros,  880);

//print shader log
void shaderLog(unsigned int shader)
{
	int infologLen = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

	if(infologLen > 1)
	{
		infoLog = new char[infologLen];
		if(infoLog == NULL)
		{
			std::cout << "ERROR: Could not allocate InfoLog buffer\n";
			exit(1);
		}
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
		std::cout<<"InfoLog: " << infoLog << "\n\n\n";
		delete[] infoLog;
	}
}

void checkOpenGLError()
{
	GLenum errCode;
	if((errCode = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OpenGl error! - " << gluErrorString(errCode);
	}
}

void initGL()
{
	glClearColor(0, 0, 0, 0);
}

void initShader()
{
	//shader code (hard coded string) *******CHANGE LATER FOR SEPARATE SHADER FILES*********
	const char* vertexShaderSource = 
		"attribute vec2 coord;\n"
		"void main() {\n"
		"	gl_Position = vec4(coord, 0.0, 1.0);\n"
		"}\n";

	const char* fragmentShaderSource = 
		"uniform vec4 color;\n"
		"void main() {\n"
		"	gl_FragColor = color;\n"
		"}\n";

	//shader ID variables
	GLuint vertexShader, fragmentShader;

	//create vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//create using source code
	glCompileShader(vertexShader);//compile shader

	std::cout << "Vertex Shader: \n";
	shaderLog(vertexShader);

	//create fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//create using source code
	glCompileShader(fragmentShader);//compile shader

	std::cout << "Fragment Shader: \n";
	shaderLog(fragmentShader);

	//create shader program and attach shaders to it

	Program = glCreateProgram();
	glAttachShader(Program, vertexShader);
	glAttachShader(Program, fragmentShader);

	//linking shader program
	glLinkProgram(Program);

	//check linking status
	int link_ok;
	glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
	if(!link_ok)
	{
		std::cout << "Error attaching shaders \n";
		return;
	}

	//getting attribute ID from created program
	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if(Attrib_vertex == -1)
	{
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return; 
	}

	//getting uniform ID
	const char* unif_name = "color";
	Unif_color = glGetUniformLocation(Program, unif_name);
	if(Unif_color == -1)
	{
		std::cout << "could not bind uniform " << unif_name << std::endl;
		return;
	}

	checkOpenGLError();
}

//initialize vertex buffer object
void initVBO()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//background rectangle vertices
	vertex rectangle[4] = 
	{
		{-1.0f, -1.0f},
		{-1.0f,  1.0f},
		{ 1.0f,  1.0f},
		{ 1.0f, -1.0f}
	};

	//put vertices into a buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);

	checkOpenGLError();
}

void freeShader()
{
	//close shader program by sending 0 value
	glUseProgram(0);
	//delete shader pogram
	glDeleteProgram(Program);
}

void freeVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//delete vertex bufer object
	glDeleteBuffers(1, &VBO);
}

void BitmapText(char *str, int wcx, int wcy)
{
	glRasterPos2i(wcx, wcy);
	for (int i=0; str[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
         
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
         
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	   	

}
     
void display()
{	
	glClear (GL_COLOR_BUFFER_BIT);
	
	glDisable(GL_DEPTH_TEST);
	//set current shader program
	glUseProgram(Program);

	static float red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
	static float green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
	static float blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
	static float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	//send uniform to shader
	glUniform4fv(Unif_color, 1, white);

	glEnableVertexAttribArray(Attrib_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_QUADS, 0, 4);
	

	glDisableVertexAttribArray(Attrib_vertex);

	glUseProgram(0);

	//checkOpenGLError(); 
	

	mGame.drawScene();

	// Write messages on right hand lower side of screen
	
	glColor3f(0, 0, 0);
	char msg[100];
	if(mBoard.gameOver)
		BitmapText("Game Over", 500, 450);
	sprintf_s(msg, "Lines: %d",mBoard.deletedLineCount);
	BitmapText(msg, 500, 500);
	sprintf_s(msg, "Level: %d", mBoard.level);
	BitmapText(msg, 500, 580);
	sprintf_s(msg, "Score: %d", mBoard.score);
	BitmapText(msg, 500, 540);

	glutSwapBuffers();
	glutPostRedisplay();
}

void keyPressed (unsigned char _key, int x, int y) {
	switch(_key)
	{
	case('x'):
		{
			// Check collision from up to down
			while (mBoard.isPossibleMovement(mGame.mPosX, mGame.mPosY, mGame.tetroType, mGame.mRotation)) { mGame.mPosY++; }
	
			mBoard.storeTetromino (mGame.mPosX, mGame.mPosY - 1, mGame.tetroType, mGame.mRotation);

			mBoard.deletePossibleLines ();			

			

			if (mBoard.isGameOver())
			{
				mBoard.gameOver = true;
				//exit(0);
			}

			mGame.createNewPiece();

			break;
		}
	case('z'):
		{
			if (mBoard.isPossibleMovement (mGame.mPosX, mGame.mPosY, mGame.tetroType, (mGame.mRotation + 1) % 4))
					mGame.mRotation = (mGame.mRotation + 1) % 4;
			glutPostRedisplay();
			break;
		}
	}

}
void specialKeyPressed (int _key, int x, int y) {
	switch(_key)
	{
	case(100)://left key
		{
			if (mBoard.isPossibleMovement (mGame.mPosX - 1, mGame.mPosY, mGame.tetroType, mGame.mRotation))
					mGame.mPosX--;	
			glutPostRedisplay();
				break;
		}
	case(102)://right key
		{
			if (mBoard.isPossibleMovement (mGame.mPosX + 1, mGame.mPosY, mGame.tetroType, mGame.mRotation))
					mGame.mPosX++;
			glutPostRedisplay();
					break;
		}
	case(103)://down key
		{
			if (mBoard.isPossibleMovement (mGame.mPosX, mGame.mPosY + 1, mGame.tetroType, mGame.mRotation))
					mGame.mPosY++;	
			glutPostRedisplay();
				break;
		}
	}

}

void update(int time1)
{
	if(mBoard.gameOver)//if game is over restart game
	{
		if(time1 < 10)
			glutTimerFunc(200, update, time1 + 1);
		else{
			mBoard.gameOver = false;
			mBoard.initBoard();
			glutTimerFunc(mBoard.timer, update, 0);
		}
		glutPostRedisplay();
	}else
	{		
		if (mBoard.isPossibleMovement (mGame.mPosX, mGame.mPosY + 1, mGame.tetroType, mGame.mRotation))
		{
			mGame.mPosY++;
		}
		else
		{
			mBoard.storeTetromino (mGame.mPosX, mGame.mPosY, mGame.tetroType, mGame.mRotation);
			mBoard.deletePossibleLines ();
			if (mBoard.isGameOver())
			{
				mBoard.gameOver = true;
			}
			mGame.createNewPiece();
		}
		if(mBoard.gameOver)
		{
			glutTimerFunc(10, update, 1);
		}else{
			glutPostRedisplay();
			glutTimerFunc(mBoard.timer, update, 0);
		}
	}
	
}



int main(int argc, char **argv) {

	//Create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);           
	glutInitWindowSize(640, 880);
	glutCreateWindow("Tetris");

	GLenum glew_status = glewInit();//do this before initializing shaders
	if(GLEW_OK != glew_status)//if glew is no initialized
	{
		std::cout<< "Error: " << glewGetErrorString(glew_status) << "\n";
		return 1;
	}

	if(!GLEW_VERSION_2_0)//check availability of OpenGL 2.0
	{
		std::cout << "No support for OpenGL 2.0 found.\n";
		return 1;
	}

	initGL();
	initVBO();
	initShader();


	glutSpecialFunc(specialKeyPressed);
	glutKeyboardFunc(keyPressed); 
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(mBoard.timer, update,0);
	glutMainLoop();

	//free resources
	freeShader();
	freeVBO();

	return 0;
}


 