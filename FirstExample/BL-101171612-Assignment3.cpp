//***************************************************************************
// BL-101171612-Assignment2.cpp by Liam Blake (C) 2018 All Rights Reserved.
//
// Assignment 3 submission.
//
// Description:
// Press F5 to see a rubix cube!	
//
//*****************************************************************************
using namespace std;
#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include <SOIL.h>
#include "glm\gtc\matrix_transform.hpp"

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)
int numSquares;
GLuint cube_vao, bonus_vao, platform_vao, ibo, points_vbo, colours_vbo, platcolors_vbo, modelID;
float rotAngle = 0.0f;

// Horizontal and vertical ortho offsets.
float osH = 0.0f, osV = 0.0f, scrollSpd = 0.25f;
int deltaTime, currentTime, lastTime = 0;
glm::mat4 mvp, view, projection;
GLfloat cameraX = 0, cameraY = 0, cameraZ = -5;
GLshort cube_indices[] = {
	// Front.
	3, 2, 1, 0,
	7, 6, 5, 4,
	11, 10, 9, 8,
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23
};

GLfloat cube_vertices[] = {
	-0.9f, -0.9f, 0.9f,		// 0.
	 0.9f, -0.9f, 0.9f,		// 1.
	 0.9f, 0.9f, 0.9f,		// 2.
	-0.9f, 0.9f, 0.9f,		// 3.

	 0.9f, -0.9f, 0.9f,		// 4.
	 0.9f, -0.9f, -0.9f,	// 5.
	 0.9f, 0.9f, -0.9f,		// 6.
	 0.9f, 0.9f, 0.9f,		// 7.

	 -0.9f, -0.9f, 0.9f,	// 8.
	 -0.9f, -0.9f, -0.9f,	// 9.
	 -0.9f, 0.9f, -0.9f,	// 10.
	 -0.9f, 0.9f, 0.9f,		// 11.

	 -0.9f, -0.9f, 0.9f,	// 12.
	 0.9f, -0.9f, 0.9f,		// 13.
	 0.9f, -0.9f, -0.9f,	// 14.
	 -0.9f, -0.9f, -0.9f,	// 15.

	-0.9f, -0.9f, -0.9f,	// 16.
	 0.9f, -0.9f, -0.9f,	// 17.
	 0.9f, 0.9f, -0.9f,		// 18.
	-0.9f, 0.9f, -0.9f,		// 19.

	 -0.9f, 0.9f, 0.9f,	    // 20.
	 0.9f, 0.9f, 0.9f,		// 21.
	 0.9f, 0.9f, -0.9f,	    // 22.
	 -0.9f, 0.9f, -0.9f,	// 23.

};

GLfloat textureCoordinates[] = {
	0.0f, 0.33f,
	0.0f, 0.66f,
	0.25f, 0.66f, // front 
	0.25f, 0.33f,

	0.25f, 0.33f,
	0.25f, 0.66f,
	0.5f, 0.66f, // front
	0.5f, 0.33f,

	0.5f, 0.33f,
	0.5f, 0.66f,
	0.75f, 0.66f, // front
	0.75f, 0.33f,

	0.75f, 0.33f,
	0.75f, 0.66f,
	1.0f, 0.66f, // front
	1.0f, 0.33f,

	0.25f, 0.0f,
	0.25f, 0.33f,
	0.5f, 0.33f, // front
	0.5f, 0.0f,

	0.25f, 0.66f,
	0.25f, 1.0f,
	0.5f, 1.0f, // front
	0.5f, 0.66f,

	
};
//////
GLshort bonus_indices[] = {
	0, 1, 2,
	3, 4, 5,
	6, 7, 8,
	9, 10, 11,
	12, 13, 14,
	15, 16, 17
};
GLfloat bonus_vertices[] = {
	-0.9f, -0.9f, -0.9f, // 0.
	0.9f, -0.9f, -0.9f,	 // 1.
	-0.9f, -0.9f, 0.9f,	 // 2.

	-0.9f, -0.9f, 0.9f,  // 3.
	0.9f, -0.9f, -0.9f,	 // 4.
	0.9f, -0.9f, 0.9f,	 // 5.

	// Left
	-0.9f, -0.9f, -0.9f, // 6.
	-0.9f, -0.9f, 0.9f,  // 7.
	0.0f, 0.6364f, 0.0f,	 // 8.

	// Back
	-0.9f, -0.9f, -0.9f, // 9.
	0.9f, -0.9f, -0.9f,	 // 10.
	0.0f, 0.6364f, 0.0f,	 // 11.
	// Right
	0.9f, -0.9f, -0.9f, // 12.
	0.9f, -0.9f, 0.9f,  // 13.
	0.0f, 0.6364f, 0.0f,    // 14.
	// Front
	-0.9f, -0.9f, 0.9f, // 15.
	0.9f, -0.9f, 0.9f,	// 16.
	0.0f, 0.6364f, 0.0f	// 17.
};

GLfloat bonusCoordinates[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,

	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	0.0713f, 1.0f,
	0.9287f, 1.0f,
	0.5f, 0.0f,

	0.0713f, 1.0f,
	0.9287f, 1.0f,
	0.5f, 0.0f,

	0.0713f, 1.0f,
	0.9287f, 1.0f,
	0.5f, 0.0f,

	0.0713f, 1.0f,
	0.9287f, 1.0f,
	0.5f, 0.0f,


};
GLfloat proj = 10.0f;
GLfloat mouseX, mouseY;
/*
1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f
*/
GLfloat colours[] = {
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f
};
/*
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f
*/

GLint width, height, width2, height2;
GLuint cube_tex = 0;
GLuint cube_tex_vbo = 0;
GLfloat colours2[] = {
	//0.9f, 0.0f, 0.0f,
	//0.9f, 0.9f, 0.9f,
	//0.9f, 0.9f, 0.9f,
	//0.9f, 0.9f, 0.9f,
	//0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.0f, 0.0f,	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.0f, 0.0f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f,
	0.9f, 0.9f, 0.9f
};
GLuint program;
ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
};

void init(void)
{
	srand((unsigned)time(NULL));
	//Specifying the name of vertex and fragment shaders.
	

	//Loading and compiling shaders
	program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up	

	modelID = glGetUniformLocation(program, "mvp");

	cube_vao = 0;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	////////////////////////////////////
	bonus_vao = 0;
	glGenVertexArrays(1, &bonus_vao);
	glBindVertexArray(bonus_vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bonus_indices), bonus_indices, GL_STATIC_DRAW);

	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bonus_vertices), bonus_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	
	//////////////////////////////////
	//platform_vao = 0;
	//glGenVertexArrays(1, &platform_vao);
	//glBindVertexArray(platform_vao);

	//glGenBuffers(1, &ibo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	//points_vbo = 0;
	//glGenBuffers(1, &points_vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//glEnableVertexAttribArray(0);

	//platcolors_vbo = 0;
	/*glGenBuffers(1, &platcolors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, platcolors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours2), colours2, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);*/
	// Textures init
	/* ========================================================================================= */
	

	unsigned char* rubicksCubeTexture = SOIL_load_image("rubiksCube.png", &width, &height, 0, SOIL_LOAD_RGB);
	

	glGenTextures(1, &cube_tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rubicksCubeTexture);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	/* In fragment shader */

	
	glGenBuffers(1, &cube_tex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	//////////////
	//GLint width, height;
	unsigned char* pyramidTexture = SOIL_load_image("bonusTexture.png", &width, &height, 0, SOIL_LOAD_RGB);
	//GLuint cube_tex = 0;
	glGenTextures(1, &cube_tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pyramidTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);


	//GLuint cube_tex_vbo = 0;
	glGenBuffers(1, &cube_tex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours2), colours2, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	/* </Textures init> */
	/* ========================================================================================= */
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.


//glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

// Enable depth test.
	glEnable(GL_DEPTH_TEST);
}

//---------------------------------------------------------------------
//x
// transformModel
//
void transformObject2(glm::vec3 s, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, s);
	mvp = projection * view * Model;
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mvp[0][0]);
}
void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	mvp = projection * view * Model;
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mvp[0][0]);
}

//---------------------------------------------------------------------
//
// display
//
unsigned char* image;
unsigned char* image2;
void display(void)
{

	static int i;
	i++;
	if (i <= 1)
	{
		image = SOIL_load_image("rubiksCube.png", &width, &height, 0, SOIL_LOAD_RGB);
		image2 = SOIL_load_image("bonusTexture.png", &width2, &height2, 0, SOIL_LOAD_RGB);
	}

	view = glm::lookAt(// 25 37.5
		glm::vec3(cameraX, cameraY, cameraZ + 17.5),		// Camera pos in World Space
		glm::vec3(cameraX, cameraY, cameraZ),	// This is necessary, we need to move the camera origin with the camera pos
		glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
	);
	projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 300.0f);
	//cout << cameraX << " " << cameraY << " " << cameraZ << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBindVertexArray(cube_vao);
	//glBindBuffer(colours_vbo, 1);
	glClearColor(53.0f / 255.0f, 81.0f / 255.0f, 92.0f / 255.0f, 1.0f);
	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	static GLfloat oppAngle = 0;
	static GLfloat oppAngle2 = 0;
	//glBindVertexArray(cube_vao);
	oppAngle += 1.5f;
	oppAngle2 -= 1.5f;


	glBindVertexArray(cube_vao); // Change vaos
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenBuffers(1, &cube_tex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	transformObject(1.0f, Y_AXIS, oppAngle, glm::vec3(3.0f, 0.0f, 0.0f));
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);


	transformObject(1.0f, Y_AXIS, oppAngle2, glm::vec3(-3.0f, 0.0f, 0.0f));
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);



	glBindVertexArray(bonus_vao); // Change vaos
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);

	glGenBuffers(1, &cube_tex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bonusCoordinates), bonusCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	transformObject(1.0f, Y_AXIS, oppAngle2, glm::vec3(0.0f, 3.0f, 0.0f));
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0);

	glutSwapBuffers(); // Instead of double buffering.
}

void idle()
{
	//glutPostRedisplay();
}

void timer(int id)
{
	glutPostRedisplay();
	glutTimerFunc(33, timer, 0);
}

void keyDown(unsigned char key, int x, int y)
{
	// Orthographic.
	switch (key)
	{
		/*case 'w':
			osV -= scrollSpd;
			break;
		case 's':
			osV += scrollSpd;
			break;
		case 'a':
			osH += scrollSpd;
			break;
		case 'd':
			osH -= scrollSpd;
			break;
		case 'l':
			rotAngle -= 5.0f;
			break;
		case 'k':
			rotAngle += 5.0f;
			break;
		case '1':
			cameraX
			break;
		case '2':
			proj -= 0.25f;
			break;*/

	case 's':
		cameraZ += 0.5f;
		break;
	case 'w':
		cameraZ -= 0.5f;
		break;
	case 'd':
		cameraX += 0.5f;
		break;
	case 'a':
		cameraX -= 0.5f;
		break;
	case 'l':
		rotAngle -= 5.0f;
		break;
	case 'k':
		rotAngle += 5.0f;
		break;
	case 'r':
		cameraY += 0.5f;
		break;
	case 'f':
		cameraY -= 0.5f;
		break;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	// Empty for now
}

void mouseMove(int x, int y)
{
	cout << "Mouse pos: " << x << "," << y << endl;
	mouseX = x - 512;
	mouseY = y - 512;
}

void mouseDown(int btn, int state, int x, int y)
{
	cout << "Clicked: " << (btn == 0 ? "left " : "right ") << (state == 0 ? "down " : "up ") <<
		"at " << x << "," << y << endl;
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
	bool looped = true;
	/*cout << "[======================================================================================]" << endl;
	cout << "[   If you don't see all the squares, press the 'r' key to go up. They are above you   ]" << endl;
	cout << "[======================================================================================]" << endl << endl;*/
	//while (looped)
	//{
	//	system("cls");
	//	cout << "[======================================================================================]" << endl;
	//	cout << "[   If you don't see all the squares, press the 'r' key to go up. They are above you   ]" << endl;
	//	cout << "[======================================================================================]" << endl << endl;
	//	cout << "Please enter the number of squares (Between 1 and 30): ";
	//	cin >> numSquares;
	//	if (cin.fail()) { cin.clear(); cin.ignore(); }
	//	if (numSquares > 0 && numSquares < 31 && !cin.fail()) {
	//		looped = false;
	//	}
	//}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Blake, Liam, 101171612");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.
	init();
	// Set all our glut functions.
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(33, timer, 0);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseDown);
	glutPassiveMotionFunc(mouseMove); // or...
	//glutMotionFunc(mouseMove); // Requires click to register.
	glutMainLoop();
}