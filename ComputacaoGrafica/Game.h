#pragma region Imports

#include "Camera.h"
#include "Model.h"

#pragma endregion

#pragma region Global_Escope

#define ESC 27

// Camera position
float x = 0.0, y = -5.0;	// initially 5 units south of origin
int sideMove = 0;			// initially camera doesn't move

// Camera direction
float lx = 0.0, ly = 1.0;	// camera points initially along y-axis
float Angle = 0.0;			// angle of rotation for the camera direction
float DeltaAngle = 0.0;		// additional angle change when dragging

// Mouse drag control
int IsDragging = 0;			// true when dragging
int DragStartX = 0;			// records the x-coordinate when dragging starts

GLfloat Ratio = 0.0;
GLsizei Width = 600, Height = 600;

// Parâmetros de câmera
Camera MainCamera = Camera(0, 0, 11, 0, 1, 1, 0, 0, 1);
Camera BackCamera = Camera(0, -80, 200, 0, 0, 0, 0, 1, 0);
Camera MapCamera = Camera(0, -115, 5, 0, 195, 0, 0, 1, 0);

// Filepaths
const char* ScenarioFile = "./Models/Car1.obj";
const char* CarFile = "./Models/Car1.obj";

// Object Models
Model ScenarioModel;
Model CarModel;

#pragma endregion

#pragma region Assign_Methods

void DefineCamera(GLsizei iniW, GLsizei iniH, GLsizei sizeW, GLsizei sizeH, Camera cam);
void DefineMainCamera();
void DefineBackCamera();
void DefineMapCamera();

#pragma endregion

#pragma region Display_Methods

/// <summary>
/// Função callback chamada para fazer o desenho.
/// </summary>
void Display(void)
{
	// Sky color is light blue
	glClearColor(0.0, 0.7, 1.0, 1.0); 

	// Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Desenho dos cenários
	DefineMainCamera();
	DefineBackCamera();
	DefineMapCamera();

	// Executa os comandos OpenGL
	glutSwapBuffers();
}

/// <summary>
/// Função callback chamada quando o tamanho da janela é alterado.
/// </summary>
/// <param name="w">Width da chamada</param>
/// <param name="h">Height da chamada</param>
void Reshape(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if (h == 0) h = 1;

	// Guarda valores de Largura e Altura globalmente
	Width = w;
	Height = h;

	float ratio = ((float)w) / ((float)h);		// window aspect ratio
	glMatrixMode(GL_PROJECTION);				// projection matrix is active
	glLoadIdentity();							// reset the projection
	gluPerspective(45.0, ratio, 0.1, 100.0);	// perspective transformation
	glMatrixMode(GL_MODELVIEW);					// return to modelview mode
}

#pragma endregion

#pragma region Cameras_Definition

void DefineMainCamera()
{
	MainCamera.Print();

	// Definição da viewport e câmera 1
	DefineCamera(0, 0, Width, Height, MainCamera);

	// Desenha o carro principal
	if (ScenarioModel.IsLoaded())
		ScenarioModel.DisplayModel();
	else
		ScenarioModel.LoadFromFile(ScenarioFile);
}

void DefineBackCamera()
{
	// Definição da viewport e câmera 2
	DefineCamera(0, 0, Width / 8, Height / 8, BackCamera);

	// Desenha o teapot com a cor corrente (wire-frame)
	glColor3f(0,1,0);
	glutWireTeapot(50.0f);
}

void DefineMapCamera()
{
	// Definição da viewport e câmera 3
	DefineCamera(Width - (Width / 8), 0, Width / 8, Height / 8, MapCamera);

	// Desenha o teapot com a cor corrente (wire-frame)
	glColor3f(1,0,0);
	glutWireTeapot(50.0f);
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

#pragma endregion

#pragma region Mouse_Methods

void MouseMove(int x, int y)
{
	if (IsDragging) {
		// update the change in angle
		DeltaAngle = (x - DragStartX) * 0.005;

		// camera's direction is set to angle + deltaAngle
		lx = -sin(Angle + DeltaAngle);
		ly = cos(Angle + DeltaAngle);

		MainCamera.HorizontalMoviment(Angle, DeltaAngle);
	}
}

/// <summary>
/// Função callback chamada para gerenciar eventos do mouse.
/// </summary>
/// <param name="button"></param>
/// <param name="state"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		// Left mouse button pressed
		if (state == GLUT_DOWN) { 
			IsDragging = 1;			// start dragging
			DragStartX = x;			// save x where button first pressed
		}
		// When state = GLUT_UP
		else { 
			Angle += DeltaAngle;	// update camera turning angle
			IsDragging = 0;			// no longer dragging
		}
	}
}

#pragma endregion

#pragma region Keyboard_Methods

void ProcessNormalKeys(unsigned char key, int xx, int yy)
{
	if (key == ESC || key == 'q' || key == 'Q') exit(0);
	if (key == ' ') MainCamera = Camera(0, 0, 11, 0, 1, 1, 0, 0, 1);
}

void PressSpecialKey(int key, int xx, int yy)
{
	switch (key) {
		case GLUT_KEY_UP: sideMove = 1; break;
		case GLUT_KEY_DOWN: sideMove = 2; break;
		case GLUT_KEY_RIGHT: sideMove = 3; break;
		case GLUT_KEY_LEFT: sideMove = 4; break;
	}
}

void ReleaseSpecialKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: sideMove = 0; break;
	case GLUT_KEY_DOWN: sideMove = 0; break;
	case GLUT_KEY_RIGHT: sideMove = 0; break;
	case GLUT_KEY_LEFT: sideMove = 0; break;
	}
}

#pragma endregion

void Update(void)
{	
	switch (sideMove) {
	// Forward
	case 1:
		MainCamera.MoveForward();
		break;
	// Back
	case 2:
		MainCamera.MoveBack();
		break;
	// Right
	case 3:
		MainCamera.MoveRight();
		DeltaAngle -= 0.05;
		lx = -sin(Angle + DeltaAngle);
		ly = cos(Angle + DeltaAngle);
		MainCamera.HorizontalMoviment(lx, ly);
		break;
	// Left
	case 4:
		MainCamera.MoveLeft();
		DeltaAngle += 0.05;
		lx = -sin(Angle + DeltaAngle);
		ly = cos(Angle + DeltaAngle);
		MainCamera.HorizontalMoviment(lx, ly);
		break;
	}

	// Redisplay everything
	glutPostRedisplay();
}

/// <summary>
/// Programa Principal.
/// </summary>
/// <returns></returns>
int Game(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	glutCreateWindow("F1 Driving Simulator");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Update);
	glutIgnoreKeyRepeat(1);                 // ignore key repeat when holding key down
	glutMouseFunc(MouseButton);             // process mouse button push/release
	glutMotionFunc(MouseMove);              // process mouse dragging motion
	glutKeyboardFunc(ProcessNormalKeys);    // process standard key clicks
	glutSpecialFunc(PressSpecialKey);       // process special key pressed
	glutSpecialUpFunc(ReleaseSpecialKey);   // process special key release

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0; // this is just to keep the compiler happy
}