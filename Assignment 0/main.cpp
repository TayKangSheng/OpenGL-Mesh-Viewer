
// Please use this chunk of code
// to load the correct opengl and glut
// for your platform
#ifdef __APPLE__
#include <GLUT/glut.h>
#elif _WIN32
#include <windows.h>
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif
// --------------------------------

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "vecmath.h"


// Included some helper macros and functions
// for those who miss the brevity of python.
#include "helpers.h"

using namespace std;

// Globals

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;


// You will need more global variables to implement color and position changes
int teapotColour = 0;
GLfloat lightposition[] = {1.0f, 1.0f, 5.0f, 1.0f};
GLfloat rtri = 2.0f;
bool spin = false;
bool fade = false;
int fadeState = 1;
GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
    {0.9, 0.5, 0.5, 1.0},
    {0.5, 0.9, 0.3, 1.0},
    {0.3, 0.8, 0.9, 1.0} };
GLfloat* currentColour = diffColors[teapotColour];
int initial_mouse_x = 0;
int initial_mouse_y = 0;
int current_mouse_x = 0;
int current_mouse_y = 0;
float camera_pos[3] = {0.0, 0.0, 5.0};

// These are convenience functions which allow us to call OpenGL
// methods on Vec3d objects
inline void glVertex(const Vector3f &a)
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a)
{ glNormal3fv(a); }

void updateSpin(int value){
    cout << "updateSpin()" << endl;
    rtri+=2.0f;
    if (rtri >= 360.0f){
        rtri = 0.0f;
    }
    glutTimerFunc(500, updateSpin, 100);
    glutPostRedisplay();
}

void updateColour(int value){
    cout << "updateColour" << endl;
    fadeState += 1;
    if (fadeState == 100) {
        fade = false;
        fadeState = 1;
        currentColour[0] = diffColors[teapotColour][0];
        currentColour[1] = diffColors[teapotColour][1];
        currentColour[2] = diffColors[teapotColour][2];
        currentColour[3] = diffColors[teapotColour][3];
        cout << "Colour fading done!" << endl;
    } else {
        if (teapotColour == 0 ){
            float stepsize0 = (diffColors[3][0] - diffColors[teapotColour][0])/100.0f;
            float stepsize1 = (diffColors[3][1] - diffColors[teapotColour][1])/100.0f;
            float stepsize2 = (diffColors[3][2] - diffColors[teapotColour][2])/100.0f;
            float stepsize3 = (diffColors[3][3] - diffColors[teapotColour][3])/100.0f;
            currentColour[0] = currentColour[0] + stepsize0;
            currentColour[1] = currentColour[1] + stepsize1;
            currentColour[2] = currentColour[2] + stepsize2;
            currentColour[3] = currentColour[3] + stepsize3;
        } else{
            float stepsize0 = (diffColors[teapotColour][0] - diffColors[teapotColour-1][0])/100.0f;
            float stepsize1 = (diffColors[teapotColour][1] - diffColors[teapotColour-1][1])/100.0f;
            float stepsize2 = (diffColors[teapotColour][2] - diffColors[teapotColour-1][2])/100.0f;
            float stepsize3 = (diffColors[teapotColour][3] - diffColors[teapotColour-1][3])/100.0f;
            currentColour[0] = currentColour[0] + stepsize0;
            currentColour[1] = currentColour[1] + stepsize1;
            currentColour[2] = currentColour[2] + stepsize2;
            currentColour[3] = currentColour[3] + stepsize3;
        }
        glutTimerFunc(10, updateColour, 10);
        glutPostRedisplay();
    }
}

void mouseFunc(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
//        cout << button << "\t" << "down" << "\t" << x << "\t" << y << endl;
        initial_mouse_x = x;
        initial_mouse_y = y;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
//        cout << button << "\t" << "up" << "\t" << x << "\t" << y << endl;
    }
}

void mouseMotion (int x,int y){
    current_mouse_x = x;
    current_mouse_y = y;
    float angle = (current_mouse_x-initial_mouse_x)/100.0f;
    float zoom = (current_mouse_y-initial_mouse_y)/1000.0f;
    
//    cout << "camera_pos[2]\t" << camera_pos[2] << endl;
//    cout << "zoom\t" << zoom << endl;
    
    if (zoom > 0.0){
        if (camera_pos[2] > 15.0){
        } else {
            camera_pos[2] += zoom;
        }
    } else{
        if (camera_pos[2] < 3.0){
            
        } else{
            camera_pos[2] += zoom;
        }
    }

    rtri += angle;
    glutPostRedisplay();
}

// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
        case 27: // Escape key
            exit(0);
            break;
        case 'c':
            // add code to change color here
            // cout << "Unhandled key press " << key << "." << endl;
            if (!fade){
                fade = true;
                teapotColour = (teapotColour+1)%3;
                glutTimerFunc(10, updateColour, 10);
            }
            break;
        case 'r':
            // Spin
            cout << "spin!" << endl;
            spin = true;
            glutTimerFunc(500, updateSpin, 100);
            break;
        default:
            cout << "Unhandled key press " << key << "." << endl;
    }
    
    // this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}



// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
        case GLUT_KEY_UP:
            // add code to change light position
            // cout << "Unhandled key press: up arrow." << endl;
            lightposition[1] = lightposition[1]-0.5;
            break;
        case GLUT_KEY_DOWN:
            // add code to change light position
            // cout << "Unhandled key press: down arrow." << endl;
            lightposition[1] = lightposition[1]+0.5;
            break;
        case GLUT_KEY_LEFT:
            // add code to change light position
            // cout << "Unhandled key press: left arrow." << endl;
            lightposition[0] = lightposition[0]-0.5;
            break;
        case GLUT_KEY_RIGHT:
            // add code to change light position
            // cout << "Unhandled key press: right arrow." << endl;
            lightposition[0] = lightposition[0]+0.5;
            break;
    }
    
    // this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;
    
    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity
    
    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2],
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    
    // Set material properties of object
    
    // Here are some colors you might use - feel free to add more
//    GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
//        {0.9, 0.5, 0.5, 1.0},
//        {0.5, 0.9, 0.3, 1.0},
//        {0.3, 0.8, 0.9, 1.0} };
    
    // Here we use the first color entry as the diffuse color

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, currentColour);
    
    // Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};
    
    // Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    
    // Set light properties
    
    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
    GLfloat Lt0pos[] = {lightposition[0], lightposition[1], lightposition[2], lightposition[3]};
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
    
    // This GLUT method draws a teapot.  You should replace
    // it with code which draws the object you loaded.
    // glutSolidTeapot(1.0);
    
//    if (spin){
        glPushMatrix();
        glRotatef(rtri,0.0f,1.0f,0.0f);
//    }
    
    for(unsigned int k=0; k < vecf.size(); k++) {
        //do something with v[0], v[1], v[2]
        int a = vecf[k][0];
        int c = vecf[k][1];
        int d = vecf[k][2];
        int f = vecf[k][3];
        int g = vecf[k][4];
        int i = vecf[k][5];
        glBegin(GL_TRIANGLES);
        glNormal3d(vecn[c-1][0], vecn[c-1][1], vecn[c-1][2]);
        glVertex3d(vecv[a-1][0], vecv[a-1][1], vecv[a-1][2]);
        glNormal3d(vecn[f-1][0], vecn[f-1][1], vecn[f-1][2]);
        glVertex3d(vecv[d-1][0], vecv[d-1][1], vecv[d-1][2]);
        glNormal3d(vecn[i-1][0], vecn[i-1][1], vecn[i-1][2]);
        glVertex3d(vecv[g-1][0], vecv[g-1][1], vecv[g-1][2]);
        glEnd();
    }
    
    // Dump the image to the screen.
    glutSwapBuffers();
    
}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }
    
    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput()
{
    
    char buffer[100];
    string s;
    Vector3f v;
    
    while (true){
        cin.getline(buffer, 100);
        stringstream ss(buffer);
        ss >> s;
        if (s == "g"){
            break;
        }
    }
    
    while (true){
        cin.getline(buffer, 100);
        stringstream ss(buffer);
        if ( ss.rdbuf()->in_avail() == 0 ){
            break;
        }
        ss >> s;
        if (s == "v"){
            ss >> v[0] >> v[1] >> v[2];
            vecv.push_back(Vector3f(v[0],v[1],v[2]));
        } else if (s == "vn") {
            ss >> v[0] >> v[1] >> v[2];
            vecn.push_back(Vector3f(v[0],v[1],v[2]));
        } else if (s == "f") {
            int a, b, c, d, e, f, g, h, i;
            char cHolder; // to skip the '/'
            vector<unsigned> vectorHolder;
            ss >> a >> cHolder >> b >> cHolder >> c;
            ss >> d >> cHolder >> e >> cHolder >> f;
            ss >> g >> cHolder >> h >> cHolder >> i;
            vectorHolder.push_back(a);
            vectorHolder.push_back(c);
            vectorHolder.push_back(d);
            vectorHolder.push_back(f);
            vectorHolder.push_back(g);
            vectorHolder.push_back(i);
            vecf.push_back(vectorHolder);
        }
    }
    
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();
    
    glutInit(&argc,argv);
    
    // We're going to animate it, so double buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    
    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");
    
    // Initialize OpenGL parameters.
    initRendering();
    
    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys
    
    // Set up callback functions for mouse press
    glutMouseFunc(mouseFunc);

    // Set up callback function for tracking mouse movement
    glutMotionFunc(mouseMotion);
    
    // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );
    
    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );
    
    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );
    
    return 0;	// This line is never reached.
}
