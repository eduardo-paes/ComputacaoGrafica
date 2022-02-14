#include <gl/glut.h>
#include <iostream>
#include "Custom.h"

float width = 640.0;
float heigth = 480.0;

/// <summary>
/// Função de inicialização da janela
/// </summary>
void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, heigth);
}

/// <summary>
/// Limpa tela e chama função para desenhar as retas.
/// </summary>
void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(2.0);

	// Denha três retas próximas
	desenhaReta(0, 0, 400, 300);
	desenhaReta(100, 100, 500, 400);
	desenhaReta(200, 200, 600, 500);

	glFlush();
}

/// <summary>
/// Método principal.
/// </summary>
/// <param name="argc">ARG 1</param>
/// <param name="argv">ARG 2</param>
/// <returns>Finalização do programa.</returns>
int Bresenham(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(width, heigth);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Algoritmo de Retas Simples com Bresenham");
	Init();
	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}
