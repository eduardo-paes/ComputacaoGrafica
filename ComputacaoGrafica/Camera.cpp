#include"Camera.h"

float speed = 1;

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
	this->angleRotation = 0;
}

void Camera::Print() {
	printf("eyeX: %f | eyeY: %f | eyeZ: %f\n", this->eyeX, this->eyeY, this->eyeZ);
	printf("centerX: %f | centerY: %f | centerZ: %f\n", this->centerX, this->centerY, this->centerZ);
	printf("upX: %f | upY: %f | upZ: %f\n\n---\n", this->upX, this->upY, this->upZ);
}

void Camera::MoveForward() {
	this->eyeZ -= speed;
}

void Camera::MoveBack() {
	this->eyeZ += speed;
}

void Camera::MoveLeft() {
	this->eyeX -= speed;
}

void Camera::MoveRight() {
	this->eyeX += speed;
}

void Camera::HorizontalMoviment(float lx, float ly) {
	this->centerX = this->eyeX + lx;
	this->centerY = this->eyeY + ly;
}
