#pragma region Imports

#include "Model.h"
#define ESC 27

#pragma endregion

#pragma region Global_Scope

// Vertex to move Scenario and Car
Vertex MoveScenario = Vertex(0, 0, 0.5);
Vertex MoveCar = Vertex(0, 0, 0.5);

// Window Dimensions
float Width = 800;
float Height = 600;

// Camera position
float x = 0, y = 0;         // x and y position for camera movement
float SideMove = 0.0;       // Get camera movement
float StartMove = 0.0;      // Start/stop car movement
float RotationAngle = 0.0;  // Camera and car angle rotation

// Camera direction
float lx = 0.0, ly = 1.0;   // camera points initially along y-axis

// Light parameters
const GLfloat LightPosition[] = { 1., 1., 1., 1. };
const GLfloat LightAmbient[] = { 0.6, 0.6, 0.6, 0.6 };
const GLfloat LightDiffuse[] = { 0.4, 0.4, 0.4, 0.4 };

// Filepaths
const char* ScenarioFile = "./Models/Scenario.obj";
const char* CarFile = "./Models/Car.obj";

// Object Models
Model ScenarioModel;
Model CarModel;

#pragma endregion

#pragma region Render_Scene_Methods

void RenderSky() {
    // Clear color and depth buffers
    glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderScene()
{
    // Enabling light for scenario
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw ground - 200x200 square colored green
    glColor3f(0.0, 0.2, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-100.0, -100.0, 0.0);
    glVertex3f(-100.0, 100.0, 0.0);
    glVertex3f(100.0, 100.0, 0.0);
    glVertex3f(100.0, -100.0, 0.0);
    glEnd();

    // Draw scenario
    if (ScenarioModel.IsLoaded())
        ScenarioModel.DisplayModel(MoveScenario, 0);
    else
        ScenarioModel.LoadFromFile(ScenarioFile);

    // Draw car
    if (CarModel.IsLoaded())
        CarModel.DisplayModel(MoveCar, RotationAngle);
    else
        CarModel.LoadFromFile(CarFile);
}

#pragma endregion

#pragma region Display_Methods

void Reshape(int w, int h)
{
    // Avoid division by zero
    if (h == 0) h = 1;

    // Store width/height information
    Width = w;
    Height = h;

    // Window aspect ratio
    float ratio = ((float)w) / ((float)h);      

    glMatrixMode(GL_PROJECTION);                    // Projection matrix is active
    glLoadIdentity();                               // Reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0); // Perspective transformation
    glMatrixMode(GL_MODELVIEW);                     // Return to modelview mode
}

void Update()
{
    if (SideMove > 0) {
        // Move to right
        if (SideMove == 1) RotationAngle -= 0.05;

        // Move to left
        else if (SideMove == 2) RotationAngle += 0.05;

        lx = -sin(RotationAngle);
        ly = cos(RotationAngle);
    }

    // Update camera position (forward and back)
    if (StartMove) {
        x += StartMove * lx * 0.1;
        y += StartMove * ly * 0.1;

        MoveCar.x += StartMove * lx * 0.1;
        MoveCar.y += StartMove * ly * 0.1;
    }
        
    // Redisplay everything
    glutPostRedisplay(); 
}

void Display()
{
    // Draw blue sky
    RenderSky();

    // Especifica o tamanho da viewport
    glViewport(0, 0, Width, Height);

    // Inicializa sistema de coordenadas de projeção
    glLoadIdentity();

    // Definição da câmera
    gluLookAt(x, y, 1, x + lx, y + ly, 0.9, 0, 0, 2);

    // Draw complete scenario with car
    RenderScene();

    // Make it all visible
    glutSwapBuffers(); 
}

#pragma endregion

#pragma region Keyboard_Methods

void ProcessNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == ESC || key == 'q' || key == 'Q') exit(0);
}

void PressSpecialKey(int key, int xx, int yy)
{
    switch (key) {
        case GLUT_KEY_UP: StartMove = 1; break;
        case GLUT_KEY_DOWN: StartMove = -1; break;
        case GLUT_KEY_RIGHT: SideMove = 1; break;
        case GLUT_KEY_LEFT: SideMove = 2; break;
    }
}

void ReleaseSpecialKey(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP: StartMove = 0; break;
        case GLUT_KEY_DOWN: StartMove = 0; break;
        case GLUT_KEY_RIGHT: SideMove = 0; break;
        case GLUT_KEY_LEFT: SideMove = 0; break;
    }
}

#pragma endregion

#pragma region Main_Method

int StartSimulator(int argc, char** argv)
{
    // General initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("Car - Driving Simulator");

    // Callbacks
    glutReshapeFunc(Reshape);               // window reshape callback
    glutDisplayFunc(Display);               // (re)display callback
    glutIdleFunc(Update);                   // incremental update
    glutIgnoreKeyRepeat(1);                 // ignore key repeat when holding key down
    glutKeyboardFunc(ProcessNormalKeys);    // process standard key clicks
    glutSpecialFunc(PressSpecialKey);       // process special key pressed
    glutSpecialUpFunc(ReleaseSpecialKey);   // process special key release
    glutMainLoop();
    
    return 0; 
}

#pragma endregion
