#include"Camera.h"
#include <stdio.h>

Camera::Camera(GLdouble eyeX,	 GLdouble eyeY,		GLdouble eyeZ,
			   GLdouble centerX, GLdouble centerY,  GLdouble centerZ,
			   GLdouble upX,	 GLdouble upY,		GLdouble upZ)
{
	this->eyeX = eyeX;
	this->eyeY = eyeY;
	this->eyeZ = eyeZ;
	this->centerX = centerX;
	this->centerY = centerY;
	this->centerZ = centerZ;
	this->upX = upX;
	this->upY = upY;
	this->upZ = upZ;
}

void Camera::Print() {
	printf("eyeX: %f | eyeY: %f | eyeZ: %f\n", this->eyeX, this->eyeY, this->eyeZ);
	printf("centerX: %f | centerY: %f | centerZ: %f\n", this->centerX, this->centerY, this->centerZ);
	printf("upX: %f | upY: %f | upZ: %f\n\n---\n", this->upX, this->upY, this->upZ);
}