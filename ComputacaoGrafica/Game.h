#pragma region Imports

#include "Camera.h"
#include "Model.h"

#pragma endregion

#pragma region Global_Escope

GLfloat angle, fAspect;
GLfloat r = 0, g = 0, b = 1;
GLsizei width = 600, height = 600;
int POS_X, POS_Y;

// Parâmetros de câmera
Camera mainCamera = Camera(0, 80, 200, 0, 0, 0, 0, 1, 0);
Camera backCamera = Camera(0, -80, 200, 0, 0, 0, 0, 1, 0);
Camera mapCamera = Camera(0, -115, 5, 0, 195, 0, 0, 1, 0);

// Filepaths
const char* CubeFile = "./Models/Car1.obj";

// Object Models
Model cube;

#pragma endregion

#pragma region Assign_Methods

void EspecifyDisplayParameters();
void DefineCamera(GLsizei iniW, GLsizei iniH, GLsizei sizeW, GLsizei sizeH, Camera cam);
void MainCamera();
void BackCamera();
void MapCamera();
void ScrollController(int, int, int, int);

#pragma endregion

/// <summary>
/// Função callback chamada para fazer o desenho.
/// </summary>
void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Desenho dos cenários
	MainCamera();
	BackCamera();
	MapCamera();

	// Executa os comandos OpenGL
	glutSwapBuffers();
}

void MainCamera()
{
	// Definição da viewport e câmera 1
	DefineCamera(0, 0, width, height, mainCamera);

	// Desenha o teapot com a cor corrente (wire-frame)
	/*glColor3f(r, g, b);
	glutWireTeapot(50.0f);*/

	// Desenha o carro principal
	if (cube.IsLoaded())
		cube.DisplayModel();
	else
		cube.LoadFromFile(CubeFile);
}

void BackCamera()
{
	// Definição da viewport e câmera 2
	DefineCamera(0, 0, width / 8, height / 8, backCamera);

	// Desenha o teapot com a cor corrente (wire-frame)
	glColor3f(0,1,0);
	glutWireTeapot(50.0f);
}

void MapCamera()
{
	// Definição da viewport e câmera 3
	DefineCamera(width - (width / 8), 0, width / 8, height / 8, mapCamera);

	// Desenha o teapot com a cor corrente (wire-frame)
	glColor3f(1,0,0);
	glutWireTeapot(50.0f);
}

/// <summary>
/// Inicializa parâmetros de rendering.
/// </summary>
void Initialize(void)
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
void EspecifyDisplayParameters()
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
void ReDisplay(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if (h == 0) h = 1;

	// Define globalmente as dimensões
	width = w;
	height = h;

	// Calcula a correção de aspecto
	fAspect = (GLfloat)width / (GLfloat)height;

	EspecifyDisplayParameters();
}

/// <summary>
/// Função callback chamada para gerenciar eventos do mouse.
/// </summary>
/// <param name="button"></param>
/// <param name="state"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void MouseController(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)  // Zoom-in
			if (angle >= 10) angle -= 5;
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN)  // Zoom-out
			if (angle <= 130) angle += 5;

	EspecifyDisplayParameters();
	glutPostRedisplay();
}

void ScrollController(int button, int dir, int x, int y)
{
	// Zoom in
	if (dir > 0)
		if (angle >= 10) angle -= 5;
	// Zoom out
	else
		if (angle <= 130) angle += 5;

	return;
}

/// <summary>
/// Função callback chamada para gerenciar eventos de teclado.
/// </summary>
/// <param name="key"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void KeyboardController(unsigned char key, int x, int y)
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
			mainCamera.MoveForward();
			break;
		// Baixo
		case 'S':
		case 's':
			mainCamera.MoveBack();
			break;
		// Esquerda
		case 'A':
		case 'a':
			mainCamera.MoveLeft();
			break;
		// Direita
		case 'D':
		case 'd':
			mainCamera.MoveRight();
			break;
		// Reset
		case ' ':
			mainCamera = Camera(0, 80, 200, 0, 0, 0, 0, 1, 0);
			break;
	}
	mainCamera.Print();
	glutPostRedisplay();
}

/// <summary>
/// Programa Principal.
/// </summary>
/// <returns></returns>
int Game(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glEnable(GLUT_MULTISAMPLE);
	glHint(GLUT_MULTISAMPLE, GL_NICEST);
	POS_X = (glutGet(GLUT_SCREEN_WIDTH) - width) >> 1;
	POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - height) >> 1;
	glutInitWindowPosition(POS_X, POS_Y);

	glutInitWindowSize(width, height);
	glutCreateWindow("Visualizacao 3D");

	Initialize();

	glutDisplayFunc(Display);
	glutReshapeFunc(ReDisplay);
	glutKeyboardFunc(KeyboardController);
	glutMouseFunc(MouseController);
	glutMainLoop();

	return 0;
}