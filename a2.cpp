#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <unistd.h>

using namespace std;

#define BASE 1.0f       // base for scaling
#define PI 3.1415926535898
#define NUM_FRAMES 11   // number of key frames
#define INFRAMES 10     // number of intermediate frames

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 1.0f, 1.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

float xangle[NUM_FRAMES], yangle[NUM_FRAMES], zangle[NUM_FRAMES];
float frontlowerangle[NUM_FRAMES], frontupperangle[NUM_FRAMES];
float backlowerangle[NUM_FRAMES], backupperangle[NUM_FRAMES];
float posx[NUM_FRAMES], posy[NUM_FRAMES], posz[NUM_FRAMES];

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

// void rotate(unsigned char key, int x, int y) {
//     if(key == 'a') {
//         xangle -= 0.5;
//         if(xangle < 0.0) {
//             xangle = 360.0;
//         }
//     }
//     else if(key == 's') {
//         xangle += 0.5;
//         if(xangle > 360.0) {
//             xangle = 0.0;
//         }
//     }
//     if(key == 'b') {
//         yangle -= 0.5;
//         if(yangle < 0.0) {
//             yangle = 360.0;
//         }
//     }
//     else if(key == 'n') {
//         yangle += 0.5;
//         if(yangle > 360.0) {
//             yangle = 0.0;
//         }
//     }
//     if(key == 'c') {
//         zangle -= 0.5;
//         if(zangle < 0.0) {
//             zangle = 360.0;
//         }
//     }
//     else if(key == 'x') {
//         zangle += 0.5;
//         if(zangle > 360.0) {
//             zangle = 0.0;
//         }
//     }
//     glutPostRedisplay();
// }

void drawCuboid(float l, float b, float h) {
    glBegin(GL_QUADS);                // Begin drawing the cuboidal base with 6 quads
        // Top face (y = 0.25f)
        // Define vertices in counter-clockwise order with normal pointing out
        glColor3f(0.0f, 1.0f, 0.0f);     
        glVertex3f( 0.5f * l * BASE, 0.5f * b * BASE, -0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE, 0.5f * b * BASE, -0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE, 0.5f * b * BASE,  0.5f * h * BASE);
        glVertex3f( 0.5f * l * BASE, 0.5f * b * BASE,  0.5f * h * BASE);

        // Bottom face (y = -0.25f)
        glColor3f(0.0f, 1.0f, 0.0f);  
        glVertex3f( 0.5f * l * BASE, -0.5f * b * BASE,  0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE, -0.5f * b * BASE,  0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE, -0.5f * b * BASE, -0.5f * h * BASE);
        glVertex3f( 0.5f * l * BASE, -0.5f * b * BASE, -0.5f * h * BASE);

        // Front face  (z = 0.5f)
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f( 0.5f * l * BASE,  0.5f * b * BASE, 0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE,  0.5f * b * BASE, 0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE, -0.5f * b * BASE, 0.5f * h * BASE);
        glVertex3f( 0.5f * l * BASE, -0.5f * b * BASE, 0.5f * h * BASE);

        // Back face (z = -0.5f)
        glColor3f(0.0f, 1.0f, 1.0f); 
        glVertex3f( 0.5f * l * BASE, -0.5f * b * BASE, -0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE, -0.5f * b * BASE, -0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE,  0.5f * b * BASE, -0.5f * h * BASE);
        glVertex3f( 0.5f * l * BASE,  0.5f * b * BASE, -0.5f * h * BASE);

        // Left face (x = -1.0f)
        glColor3f(1.0f, 1.0f, 0.0f);  
        glVertex3f(-0.5f * l * BASE,  0.5f * b * BASE,  0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE,  0.5f * b * BASE, -0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE, -0.5f * b * BASE, -0.5f * h * BASE);
        glVertex3f(-0.5f * l * BASE, -0.5f * b * BASE,  0.5f * h * BASE);

        // Right face (x = 1.0f)
        glColor3f(1.0f, 1.0f, 0.0f);  
        glVertex3f(0.5f * l * BASE,  0.5f * b * BASE, -0.5f * h * BASE);
        glVertex3f(0.5f * l * BASE,  0.5f * b * BASE,  0.5f * h * BASE);
        glVertex3f(0.5f * l * BASE, -0.5f * b * BASE,  0.5f * h * BASE);
        glVertex3f(0.5f * l * BASE, -0.5f * b * BASE, -0.5f * h * BASE);
    glEnd();  
}

void drawJoint() {
    glColor3f(0.2f, 0.2f, 0.0f);  
    glutSolidSphere(0.1f, 20, 20);
}

void backTopLeg() {
    glColor3f(0.50f, 0.31f, 0.0f);  
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glRotatef(-75.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic,0.1f,0.1f,1.0f,20,20);
}

void backLowerLeg() {
    glColor3f(0.50f, 0.31f, 0.0f);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glRotatef(150.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadratic,0.1f,0.1f,1.0f,20,20);
}

void backFeet() {
    glColor3f(0.50f, 0.31f, 0.0f);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glRotatef(10.0f, 0.0f, 1.0f, 0.0f);    
    gluCylinder(quadratic,0.1f,0.01f,0.5f,20,20);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic,0.1f,0.01f,0.5f,20,20);
    glRotatef(60.0f, -1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic,0.1f,0.01f,0.5f,20,20);
}

void frontTopLeg() {
    glColor3f(0.50f, 0.31f, 0.0f);  
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glRotatef(45.0f, 0.0f, 0.0f, -1.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic,0.1f,0.1f,0.5f,20,20);
}

void frontLowerLeg() {
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glColor3f(0.50f, 0.31f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadratic,0.1f,0.1f,0.5f,20,20);
}

void frontFeet() {
    glColor3f(0.50f, 0.31f, 0.0f);  
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadratic,0.1f,0.01f,0.5f,20,20);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic,0.1f,0.01f,0.5f,20,20);
    glRotatef(60.0f, -1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic,0.1f,0.01f,0.5f,20,20);
}

void drawEye() {
    glColor3f(0.73f, 0.0f, 0.0f);  
    glutSolidSphere(0.1, 20, 20);
}

void displayFrame(int i, int j) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);       // To operate on model-view matrix

    glLoadIdentity();                 // Reset the model-view matrix by loading identity
    
    glPushMatrix();
        glTranslatef(posx[i-1] + j*(posx[i] - posx[i-1])/INFRAMES, 
                     posy[i-1] + j*(posy[i] - posy[i-1])/INFRAMES, 
                     posz[i-1] + j*(posz[i] - posz[i-1])/INFRAMES);  // Move right and into the screen
        // glRotatef(5, 0.0, 0.0, 1.0);    // Rotate for a better visibility (may comment it out)
        // glRotatef(90, 1.0, 0.0, 0.0);
        glRotatef(xangle[i-1] + j*(xangle[i] - xangle[i-1])/INFRAMES, 1.0, 0.0, 0.0);
        glRotatef(yangle[i-1] + j*(yangle[i] - yangle[i-1])/INFRAMES, 0.0, 1.0, 0.0);
        glRotatef(zangle[i-1] + j*(zangle[i] - zangle[i-1])/INFRAMES, 0.0, 0.0, 1.0);
        drawCuboid(2.0f, 0.5f, 1.0f);

        glPushMatrix();

            // right back joint
            glTranslatef(-0.4f, 0.0f, 0.55f);
            drawJoint();
            
            // right back leg
            glPushMatrix();
                glRotatef(backupperangle[i-1] + j*(backupperangle[i] - backupperangle[i-1])/INFRAMES, 0.0, 0.0, 1.0);
                backTopLeg();
                
                glTranslatef(0.0f, 0.0f, 1.0f);
                drawJoint();
                
                glPushMatrix();
                    glRotatef(backlowerangle[i-1] + j*(backlowerangle[i] - backlowerangle[i-1])/INFRAMES, 0.0, -1.0, 0.0);
                    backLowerLeg();

                    glTranslatef(0.0f, 0.0f, 1.0f);
                    drawJoint();

                    backFeet();
                glPopMatrix();
                
            glPopMatrix();

            // right front joint
            glTranslatef(1.2f, 0.0f, 0.0f);
            drawJoint();
            
            // right front leg
            glPushMatrix();
                glRotatef(frontupperangle[i-1] + j*(frontupperangle[i] - frontupperangle[i-1])/INFRAMES, 0.0, 0.0, 1.0);
                frontTopLeg();

                glTranslatef(0.0f, 0.0f, 0.5f);
                drawJoint();

                glPushMatrix();
                    glRotatef(frontlowerangle[i-1] + j*(frontlowerangle[i] - frontlowerangle[i-1])/INFRAMES, 0.0, -1.0, 0.0);
                    frontLowerLeg();

                    glTranslatef(0.0f, 0.0f, 0.5f);
                    drawJoint();

                    frontFeet();
                glPopMatrix();

            glPopMatrix();

            // left front joint
            glTranslatef(0.0f, 0.0f, -1.1f);
            drawJoint();
            
            // left front leg
            glPushMatrix();
                glRotatef(frontupperangle[i-1] + j*(frontupperangle[i] - frontupperangle[i-1])/INFRAMES, 0.0, 0.0, 1.0);
                frontTopLeg();

                glTranslatef(0.0f, 0.0f, 0.5f);
                drawJoint();
                
                glPushMatrix();
                    glRotatef(frontlowerangle[i-1] + j*(frontlowerangle[i] - frontlowerangle[i-1])/INFRAMES, 0.0, -1.0, 0.0);
                    frontLowerLeg();

                    glTranslatef(0.0f, 0.0f, 0.5f);
                    drawJoint();

                    frontFeet();
                glPopMatrix();

            glPopMatrix();

            // left back joint
            glTranslatef(-1.2f, 0.0f, -0.0f);
            drawJoint();
            
            // left back leg
            glPushMatrix();
                glRotatef(backupperangle[i-1] + j*(backupperangle[i] - backupperangle[i-1])/INFRAMES, 0.0, 0.0, 1.0);
                backTopLeg();

                glTranslatef(0.0f, 0.0f, 1.0f);
                drawJoint();

                glPushMatrix();
                    glRotatef(backlowerangle[i-1] + j*(backlowerangle[i] - backlowerangle[i-1])/INFRAMES, 0.0, -1.0, 0.0);
                    backLowerLeg();

                    glTranslatef(0.0f, 0.0f, 1.0f);
                    drawJoint();

                    backFeet();
                glPopMatrix();

            glPopMatrix();

        glPopMatrix();

        // draw cuboidal face
        glPushMatrix();    
            glTranslatef(1.125f, 0.0f, 0.0f);
            // draw face
            drawCuboid(0.25f, 0.5f, 0.5f);

            // draw eyes
            glTranslatef(0.125f, 0.25f, 0.2f);
            drawEye();
            
            glTranslatef(0.0f, 0.0f, -0.4f);
            drawEye();
        
        glPopMatrix();
    
    glPopMatrix();
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void display() {
    for(int i = 1; i < NUM_FRAMES; i++) {
        for(int j = 0; j < INFRAMES; j++) {
            displayFrame(i, j);
        }
        usleep(100000);
    }
    glutPostRedisplay();
}

void reshape(GLsizei width, GLsizei height) {  
   // aspect ratio of the new window
    if (height == 0) {
        height = 1;                // divide by 0 !!!Alert!!!
    }
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();             // load identity for projection matrix
    // enable perspective projection with fov, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void readFrame() {
    ifstream in("frame.txt");
    string framename;
    int i = 0;
    while(in >> framename) {
        cout << framename << endl;
        string name, matname, partname;
        in >> name;
        cout << "full body" << endl;
        in >> matname >> xangle[i];
        in >> matname >> yangle[i];
        in >> matname >> zangle[i];
        in >> matname >> posx[i] >> posy[i] >> posz[i];
        in >> name;
        cout << "front leg" << endl;
        in >> partname;
        in >> matname >> frontupperangle[i];
        in >> partname;
        in >> matname >> frontlowerangle[i];
        in >> name;
        cout << "back leg" << endl;
        in >> partname;
        in >> matname >> backupperangle[i];
        in >> partname;
        in >> matname >> backlowerangle[i];
        i++;
    }
    in.close();
}

int main ( int argc, char * argv[] ) {

    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode
    glutInitWindowSize(640, 480);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow("Frog's Model");          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    // glutKeyboardFunc(rotate);
    readFrame();
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}
