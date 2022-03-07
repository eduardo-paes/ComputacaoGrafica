#include <iostream>
#include <gl/glut.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define ESC 27

//----------------------------------------------------------------------
// Global variables
//
// The coordinate system is set up so that the (x,y)-coordinate plane
// is the ground, and the z-axis is directed upwards. The y-axis points
// to the north and the x-axis points to the east.
//
// The values (x,y) are the current camera position. The values (lx, ly)
// point in the direction the camera is looking. The variables angle and
// deltaAngle control the camera's angle. The variable deltaMove
// indicates the amount of incremental motion for the camera with each
// redraw cycle. The variables isDragging and xDragStart are used to
// monitor the mouse when it drags (with the left button down).
//----------------------------------------------------------------------

// Camera position
float x = 0.0, y = -5.0; // initially 5 units south of origin
float sideMove = 0.0; // initially camera doesn't move

// Camera direction
float lx = 0.0, ly = 1.0; // camera points initially along y-axis
float Angle = 0.0; // angle of rotation for the camera direction
float DeltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int IsDragging = 0; // true when dragging
int DragStartX = 0; // records the x-coordinate when dragging starts

//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------
void Reshape(int w, int h)
{
    float ratio = ((float)w) / ((float)h); // window aspect ratio
    glMatrixMode(GL_PROJECTION); // projection matrix is active
    glLoadIdentity(); // reset the projection
    gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW); // return to modelview mode
    glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

//----------------------------------------------------------------------
// Draw one snowmen (at the origin)
//
// A snowman consists of a large body sphere and a smaller head sphere.
// The head sphere has two black eyes and an orange conical nose. To
// better create the impression they are sitting on the ground, we draw
// a fake shadow, consisting of a dark circle under each.
//
// We make extensive use of nested transformations. Everything is drawn
// relative to the origin. The snowman's eyes and nose are positioned
// relative to a head sphere centered at the origin. Then the head is
// translated into its final position. The body is drawn and translated
// into its final position.
//----------------------------------------------------------------------
void drawSnowman()
{
    // Draw body (a 20x20 spherical mesh of radius 0.75 at height 0.75)
    glColor3f(1.0, 1.0, 1.0); // set drawing color to white
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.75);
    glutSolidSphere(0.75, 20, 20);
    glPopMatrix();

    // Draw the head (a sphere of radius 0.25 at height 1.75)
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.75); // position head
    glutSolidSphere(0.25, 20, 20); // head sphere

    // Draw Eyes (two small black spheres)
    glColor3f(0.0, 0.0, 0.0); // eyes are black
    glPushMatrix();
    glTranslatef(0.0, -0.18, 0.10); // lift eyes to final position
    glPushMatrix();
    glTranslatef(-0.05, 0.0, 0.0);
    glutSolidSphere(0.05, 10, 10); // right eye
    glPopMatrix();
    glPushMatrix();
    glTranslatef(+0.05, 0.0, 0.0);
    glutSolidSphere(0.05, 10, 10); // left eye
    glPopMatrix();
    glPopMatrix();

    // Draw Nose (the nose is an orange cone)
    glColor3f(1.0, 0.5, 0.5); // nose is orange
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to point along -y
    glutSolidCone(0.08, 0.5, 10, 2); // draw cone
    glPopMatrix();
    glPopMatrix();

    // Draw a faux shadow beneath snow man (dark green circle)
    glColor3f(0.0, 0.5, 0.0);
    glPushMatrix();
    glTranslatef(0.2, 0.2, 0.001);    // translate to just above ground
    glScalef(1.0, 1.0, 0.0); // scale sphere into a flat pancake
    glutSolidSphere(0.75, 20, 20); // shadow same size as body
    glPopMatrix();
}

//----------------------------------------------------------------------
// Update with each idle event
//
// This incrementally moves the camera and requests that the scene be
// redrawn.
//----------------------------------------------------------------------
void Update(void)
{
    if (sideMove) { // update camera position
        x += sideMove * lx * 0.1;
        y += sideMove * ly * 0.1;
    }
    glutPostRedisplay(); // redisplay everything
}

//----------------------------------------------------------------------
// Draw the entire scene
//
// We first update the camera location based on its distance from the
// origin and its direction.
//----------------------------------------------------------------------
void RenderScene(void)
{
    int i, j;

    // Clear color and depth buffers
    glClearColor(0.0, 0.7, 1.0, 1.0); // sky color is light blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt(x, y, 1.0, x + lx, y + ly, 1.0, 0.0, 0.0, 1.0);

    #pragma region Drawing

    // Draw ground - 200x200 square colored green
    glColor3f(0.0, 0.7, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-100.0, -100.0, 0.0);
    glVertex3f(-100.0, 100.0, 0.0);
    glVertex3f(100.0, 100.0, 0.0);
    glVertex3f(100.0, -100.0, 0.0);
    glEnd();

    // Draw 36 snow men
    for (i = -3; i < 3; i++) {
        for (j = -3; j < 3; j++) {
            glPushMatrix();
            glTranslatef(i * 7.5, j * 7.5, 0);
            drawSnowman();
            glPopMatrix();
        }
    }

    #pragma endregion

    glutSwapBuffers(); // Make it all visible
}

#pragma region Moves_and_Main

//----------------------------------------------------------------------
// User-input callbacks
//
// processNormalKeys: ESC, q, and Q cause program to exit
// pressSpecialKey: Up arrow = forward motion, down arrow = backwards
// releaseSpecialKey: Set incremental motion to zero
//----------------------------------------------------------------------
void ProcessNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == ESC || key == 'q' || key == 'Q') exit(0);
}

void PressSpecialKey(int key, int xx, int yy)
{
    switch (key) {
    case GLUT_KEY_UP: sideMove = 1.0; break;
    case GLUT_KEY_DOWN: sideMove = -1.0; break;
    }
}

void ReleaseSpecialKey(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP: sideMove = 0.0; break;
    case GLUT_KEY_DOWN: sideMove = 0.0; break;
    }
}

//----------------------------------------------------------------------
// Process mouse drag events
//
// This is called when dragging motion occurs. The variable
// angle stores the camera angle at the instance when dragging
// started, and deltaAngle is a additional angle based on the
// mouse movement since dragging started.
//----------------------------------------------------------------------
void MouseMove(int x, int y)
{
    if (IsDragging) { // only when dragging
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
        if (state == GLUT_DOWN) { // left mouse button pressed
            IsDragging = 1; // start dragging
            DragStartX = x; // save x where button first pressed
        }
        else { /* (state = GLUT_UP) */
            Angle += DeltaAngle; // update camera turning angle
            IsDragging = 0; // no longer dragging
        }
    }
}

//----------------------------------------------------------------------
// Main program  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
int DisplaySnowMan(int argc, char** argv)
{
    printf("\n\
-----------------------------------------------------------------------\n\
  OpenGL Sample Program:\n\
  - Drag mouse left-right to rotate camera\n\
  - Hold up-arrow/down-arrow to move camera forward/backward\n\
  - q or ESC to quit\n\
-----------------------------------------------------------------------\n");

    // general initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 400);
    glutCreateWindow("OpenGL/GLUT Sampe Program");

    // register callbacks
    glutReshapeFunc(Reshape);            // window reshape callback
    glutDisplayFunc(RenderScene);           // (re)display callback
    glutIdleFunc(Update);                   // incremental update
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

#pragma endregion
