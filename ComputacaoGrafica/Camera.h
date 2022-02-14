#ifndef CAMERA_CLASS
#define CAMERA_CLASS

#include <gl/glut.h>

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

	// Especifica posição do observador e do alvo [posCam(3), vetUp(3), vetAlvo(3)]
	Camera(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, 
		   GLdouble centerX, GLdouble centerY, GLdouble centerZ, 
		   GLdouble upX, GLdouble upY, GLdouble upZ);

	void Print();
};

#endif