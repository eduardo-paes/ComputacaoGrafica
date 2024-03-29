#pragma region Imports

#include "Model.h"
#define ESC 27

#pragma endregion

#pragma region Global_Scope

Vertex MoveObject = Vertex(0, 0, 0.5);

// Window Dimensions
float Width = 800;
float Height = 600;

// Camera position
float x = 0, y = -5;        // initially 5 units south of origin
float sideMove = 0.0;      // get camera movement
float start = 0.0;          // start/stop car movement
float centerZ = 1;

// Camera direction
float lx = 0.0, ly = 1.0;   // camera points initially along y-axis
float Angle = 0.0;          // angle of rotation for the camera direction
float DeltaAngle = 0.0;     // additional angle change when dragging

// Mouse drag control
int IsDragging = 0;         // true when dragging
int DragStartX = 0;         // records the x-coordinate when dragging starts

// Filepaths
const char* ScenarioFile = "./Models/Scenario.obj";
const char* CarFile = "./Models/IndyCar.obj";

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
    // Draw ground - 200x200 square colored green
    glColor3f(0.0, 0.2, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-100.0, -100.0, 0.0);
    glVertex3f(-100.0, 100.0, 0.0);
    glVertex3f(100.0, 100.0, 0.0);
    glVertex3f(100.0, -100.0, 0.0);
    glEnd();

    // Draw main car
    if (CarModel.IsLoaded())
        CarModel.DisplayModel(MoveObject);
    else
        CarModel.LoadFromFile(ScenarioFile);

    // TODO: Draw complete scenario
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

    float ratio = ((float)w) / ((float)h);      // Window aspect ratio
    glMatrixMode(GL_PROJECTION);                // Projection matrix is active
    glLoadIdentity();                           // Reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0);    // Perspective transformation
    glMatrixMode(GL_MODELVIEW);                 // Return to modelview mode
}

void Update()
{
    if (sideMove > 0) {
        if (sideMove == 1) DeltaAngle -= 0.05;
        else if (sideMove == 2) DeltaAngle += 0.05;
        lx = -sin(Angle + DeltaAngle);
        ly = cos(Angle + DeltaAngle);
    }

    // Update camera position
    if (start) {
        x += start * lx * 0.1;
        y += start * ly * 0.1;
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

    // Inicializa sistema de coordenadas de proje��o
    glLoadIdentity();

    // Defini��o da c�mera
    gluLookAt(x, y, 1, x + lx, y + ly, centerZ, 0, 0, 1);

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
    if (key == 'w') centerZ += 0.1;
    if (key == 's') centerZ -= 0.1;
}

void PressSpecialKey(int key, int xx, int yy)
{
    switch (key) {
    case GLUT_KEY_UP: start = 1; break;
    case GLUT_KEY_DOWN: start = -1; break;
    case GLUT_KEY_RIGHT: sideMove = 1; break;
    case GLUT_KEY_LEFT: sideMove = 2; break;
    }
}

void ReleaseSpecialKey(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP: start = 0; break;
    case GLUT_KEY_DOWN: start = 0; break;
    case GLUT_KEY_RIGHT: sideMove = 0; break;
    case GLUT_KEY_LEFT: sideMove = 0; break;
    }
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
    }
}

void MouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            IsDragging = 1; // start dragging
            DragStartX = x; // save x where button first pressed
        }
        else {
            Angle += DeltaAngle; // update camera turning angle
            IsDragging = 0; // no longer dragging
        }
    }
}

#pragma endregion

#pragma region Main_Method

int DisplayCarGame(int argc, char** argv)
{
    // General initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("F1 - Driving Simulator");

    // Callbacks
    glutReshapeFunc(Reshape);               // window reshape callback
    glutDisplayFunc(Display);               // (re)display callback
    glutIdleFunc(Update);                   // incremental update
    glutIgnoreKeyRepeat(1);                 // ignore key repeat when holding key down
    glutMouseFunc(MouseButton);             // process mouse button push/release
    glutMotionFunc(MouseMove);              // process mouse dragging motion
    glutKeyboardFunc(ProcessNormalKeys);    // process standard key clicks
    glutSpecialFunc(PressSpecialKey);       // process special key pressed
    glutSpecialUpFunc(ReleaseSpecialKey);   // process special key release

    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    
    return 0; 
}

#pragma endregion
