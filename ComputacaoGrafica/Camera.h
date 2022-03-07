#ifndef CAMERA_CLASS
#define CAMERA_CLASS

#include <iostream>
#include <gl/glut.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

class Camera
{
public:
	GLdouble eyeX;
	GLdouble eyeY;
	GLdouble eyeZ;
	GLdouble centerX;
	GLdouble centerY;
	GLdouble centerZ;
	GLdouble upX;
	GLdouble upY;
	GLdouble upZ;
	GLdouble angleRotation;

	// Especifica posição do observador e do alvo [posCam(3), vetUp(3), vetAlvo(3)]
	Camera(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, 
		   GLdouble centerX, GLdouble centerY, GLdouble centerZ, 
		   GLdouble upX, GLdouble upY, GLdouble upZ);

	void Print();
	void MoveForward();
	void MoveBack();
	void MoveRight();
	void MoveLeft();
	void HorizontalMoviment(float lx, float ly);
};

#endif