#include <gl/glut.h>
#include <iostream>

/// <summary>
/// Renderiza ponto conforme coordenada x, y
/// </summary>
/// <param name="x">Coord. x</param>
/// <param name="y">Coord. y</param>
void setPixel(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y); //Passagem da posição do ponto (2 int)
	glEnd();
}

/// <summary>
/// Algoritmo simples para desenhar uma elipse.
/// </summary>
void desenhaElipe()
{
	int rx = 100, ry = 125;
	int xCenter = 250, yCenter = 250;

	float x = 0;
	float y = ry;
	float p1 = ry * ry - (rx * rx) * ry + (rx * rx) * (0.25);
	float dx = 2 * (ry * ry) * x;
	float dy = 2 * (rx * rx) * y;

	glColor3ub(rand() % 255, rand() % 255, rand() % 255);

	while (dx < dy)
	{
		setPixel(xCenter + x, yCenter + y);
		setPixel(xCenter - x, yCenter + y);
		setPixel(xCenter + x, yCenter - y);
		setPixel(xCenter - x, yCenter - y);
		if (p1 < 0)
		{
			x = x + 1;
			dx = 2 * (ry * ry) * x;
			p1 = p1 + dx + (ry * ry);
		}
		else
		{
			x = x + 1;
			y = y - 1;
			dx = 2 * (ry * ry) * x;
			dy = 2 * (rx * rx) * y;
			p1 = p1 + dx - dy + (ry * ry);
		}
	}

	glFlush();

	float p2 = (ry * ry) * (x + 0.5) * (x + 0.5) + (rx * rx) * (y - 1) * (y - 1) - (rx * rx) * (ry * ry);

	glColor3ub(rand() % 255, rand() % 255, rand() % 255);

	while (y > 0)
	{
		setPixel(xCenter + x, yCenter + y);
		setPixel(xCenter - x, yCenter + y);
		setPixel(xCenter + x, yCenter - y);
		setPixel(xCenter - x, yCenter - y);

		if (p2 > 0)
		{
			x = x;
			y = y - 1;
			dy = 2 * (rx * rx) * y;
			p2 = p2 - dy + (rx * rx);
		}
		else
		{
			x = x + 1;
			y = y - 1;
			dy = dy - 2 * (rx * rx);
			dx = dx + 2 * (ry * ry);
			p2 = p2 + dx -
				dy + (rx * rx);
		}
	}

	glFlush();
}

/// <summary>
/// Desenha uma reta utilizando o algoritmo de Bresenham.
/// </summary>
void desenhaReta(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int x = x1, y = y1;
	int d = 2 * dy - dx;

	/// <summary>
	/// E é o ponto abaixo do ponto médio.
	/// </summary>
	int incE = 2 * dy;

	/// <summary>
	/// NE é o ponto acima do ponto médio.
	/// </summary>
	int incNE = 2 * (dy - dx);

	while (x < x2) {
		//--Escolhe E
		if (d <= 0) {
			d += incE;
			x++;
		}
		//--Escolhe NE
		else {
			d += incNE;
			x++;
			y++;
		}
		setPixel(x, y);
	}

	glColor3ub(rand() % 255, rand() % 255, rand() % 255);
}