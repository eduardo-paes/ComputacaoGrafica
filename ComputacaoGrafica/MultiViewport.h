#pragma region Importações
#include <gl/glut.h>
#include "Camera.h"
#pragma endregion

#pragma region Escopo Global
GLfloat angle, fAspect;
GLfloat r = 0, g = 0, b = 1;
GLsizei width = 600, height = 600;

// Parâmetros de câmera
Camera cam1 = Camera(0, 80, 200, 0, 0, 0, 0, 1, 0);
Camera cam2 = Camera(0, -80, 200, 0, 0, 0, 0, 1, 0);
Camera cam3 = Camera(0, -115, 5, 0, 195, 0, 0, 1, 0);
#pragma endregion

#pragma region Assinaturas
void EspecificaParametrosVisualizacao();
void DefineCamera(GLsizei iniW, GLsizei iniH, GLsizei sizeW, GLsizei sizeH, Camera cam);
void CameraPrincipal();
void CameraRetrovisor();
void CameraMapa();
#pragma endregion

/// <summary>
/// Função callback chamada para fazer o desenho.
/// </summary>
void Desenha(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Desenho dos cenários
	CameraPrincipal();
	CameraRetrovisor();
	CameraMapa();

	// Executa os comandos OpenGL
	glutSwapBuffers();
}

void CameraPrincipal()
{
	// Definição da viewport e câmera 1
	DefineCamera(0, 0, width, height, cam1);

	// Desenha o teapot com a cor corrente (wire-frame)
	glColor3f(r, g, b);
	glutWireTeapot(50.0f);
}

void CameraRetrovisor()
{
	// Definição da viewport e câmera 2
	DefineCamera(0, 0, width / 8, height / 8, cam2);

	// Desenha o teapot com a cor corrente (wire-frame)
	glColor3f(0,1,0);
	glutWireTeapot(50.0f);
}

void CameraMapa()
{
	// Definição da viewport e câmera 3
	DefineCamera(width - (width / 8), 0, width / 8, height / 8, cam3);

	// Desenha o teapot com a cor corrente (wire-frame)
	glColor3f(1,0,0);
	glutWireTeapot(50.0f);
}

/// <summary>
/// Inicializa parâmetros de rendering.
/// </summary>
void Inicializa(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	angle = 90;
}

/// <summary>
/// Define onde será reproduzida a câmera.
/// </summary>
/// <param name="iniW">Ponto de início no eixo X</param>
/// <param name="iniH">Ponto de início no eixo Y</param>
/// <param name="sizeW">Largura da Viewport</param>
/// <param name="sizeH">Altura da Viewport</param>
void DefineCamera(GLsizei iniW, GLsizei iniH, GLsizei sizeW, GLsizei sizeH, Camera cam)
{
	// Especifica o tamanho da viewport
	glViewport(iniW, iniH, sizeW, sizeH);

	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Definição da câmera
	gluLookAt(cam.eyeX,		cam.eyeY,		cam.eyeZ, 
			  cam.centerX,	cam.centerY,	cam.centerZ, 
			  cam.upX,		cam.upY,		cam.upZ);
}

/// <summary>
/// Função usada para especificar o volume de visualização.
/// </summary>
void EspecificaParametrosVisualizacao()
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);

	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(angle, fAspect, 0.5, 500);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);

	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
}

/// <summary>
/// Função callback chamada quando o tamanho da janela é alterado.
/// </summary>
/// <param name="w">Width da chamada</param>
/// <param name="h">Height da chamada</param>
void FuncaoRedesenho(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if (h == 0) h = 1;

	// Define globalmente as dimensões
	width = w;
	height = h;

	// Calcula a correção de aspecto
	fAspect = (GLfloat)width / (GLfloat)height;

	EspecificaParametrosVisualizacao();
}

/// <summary>
/// Função callback chamada para gerenciar eventos do mouse.
/// </summary>
/// <param name="button"></param>
/// <param name="state"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)  // Zoom-in
			if (angle >= 10) angle -= 5;
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN)  // Zoom-out
			if (angle <= 130) angle += 5;

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

/// <summary>
/// Função callback chamada para gerenciar eventos de teclado.
/// </summary>
/// <param name="key"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void GerenciaTeclado(unsigned char key, int x, int y)
{
	switch (key) {
		// Muda a cor corrente para vermelho
		case 'R':
		case 'r':
			r = 1;
			g = 0;
			b = 0;
			break;
		// Muda a cor corrente para verde
		case 'G':
		case 'g':
			r = 0;
			g = 1;
			b = 0;
			break;
		// Muda a cor corrente para azul
		case 'B':
		case 'b':
			r = 0;
			g = 0;
			b = 1;
			break;
		// Cima
		case 'W':
		case 'w':
			cam1.eyeY += 5;
			cam1.eyeZ += 5;
			cam1.centerY += 5;
			cam1.centerZ += 5;
			break;
		// Baixo
		case 'S':
		case 's':
			cam1.eyeY -= 5;
			cam1.eyeZ -= 5;
			cam1.centerY -= 5;
			cam1.centerZ -= 5;
			break;
		// Esquerda
		case 'A':
		case 'a':
			cam1.eyeX -= 5;
			cam1.centerX -= 5;
			break;
		// Direita
		case 'D':
		case 'd':
			cam1.eyeX += 5;
			cam1.centerX += 5;
			break;
		// Reset
		case ' ':
			cam1 = Camera(0, 80, 200, 0, 0, 0, 0, 1, 0);
			break;
	}
	cam1.Print();
	glutPostRedisplay();
}

/// <summary>
/// Programa Principal.
/// </summary>
/// <returns></returns>
int MultiViewport(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Visualizacao 3D");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(FuncaoRedesenho);
	glutKeyboardFunc(GerenciaTeclado);
	glutMouseFunc(GerenciaMouse);
	Inicializa();
	glutMainLoop();
	return 0;
}