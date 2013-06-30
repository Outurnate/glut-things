#include <GL/gl.h>

#define _USE_MATH_DEFINES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <ctgmath>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define BGW 256
#define BGH 256

static float ar;
static int width, height;
static bool mode = false;
static GLuint testtex;

static void loadTexture(GLuint *tex, int width, int height)
{
    glGenTextures(0, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

static void updateTexture(GLuint tex, int width, int height, char* data)
{
    //char* data = new char[width * height * 3];
    glBindTexture(GL_TEXTURE_2D, tex);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
}

static void errcheck()
{
    switch(glGetError())
    {
    case GL_INVALID_VALUE:
        std::cout << "GL_INVALID_VALUE" << std::endl;
        break;
    case GL_INVALID_ENUM:
        std::cout << "GL_INVALID_ENUM" << std::endl;
        break;
    case GL_INVALID_OPERATION:
        std::cout << "GL_INVALID_OPERATION" << std::endl;
        break;
    case GL_STACK_OVERFLOW:
        std::cout << "GL_STACK_OVERFLOW" << std::endl;
        break;
    case GL_STACK_UNDERFLOW:
        std::cout << "GL_STACK_UNDERFLOW" << std::endl;
        break;
    case GL_OUT_OF_MEMORY:
        std::cout << "GL_OUT_OF_MEMORY" << std::endl;
        break;
    }
}

static void resize(int w, int h)
{
    width = w;
    height = h;
    ar = (float) width / (float) height;
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    glViewport(0, 0, width, height);

    glClearColor(mode, mode, mode, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3d(1, 1, 1);
    char* data = new char[BGW * BGH * 3];
    unsigned char r, g, b;
    double rp, gp, bp;
    r = std::max(255 - fabs(fmodf((t / 5) * 765, 765) - 255), 0.0);
    g = std::max(255 - fabs(fmodf(((t / 5) + (1.0 / 3.0)) * 765, 765) - 255), 0.0);
    b = std::max(255 - fabs(fmodf(((t / 5) + (2.0 / 3.0)) * 765, 765) - 255), 0.0);
    rp = 1 + (sin(t / 5) / 2);
    gp = 1 + (cos(t / 5) / 2);
    bp = 1 + (tan(t / 5) / 2);
    for (unsigned i = 0; i < (BGW * BGH); ++i)
    {
        int x = (i % BGW);
        int y = (i / BGW);
        data[i * 3] =       std::min((int)(r - (pow(x, rp) + pow(y, rp))), 255);
        data[(i * 3) + 1] = std::min((int)(g - (pow(x, gp) + pow(y, gp))), 255);
        data[(i * 3) + 2] = std::min((int)(b - (pow(x, bp) + pow(y, bp))), 255);
    }
    updateTexture(testtex, BGW, BGH, data);
    delete[] data;

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, testtex);
    glBegin(GL_QUADS);
    glVertex2f(0, height);      glTexCoord2f (0.0, 1.0);
    glVertex2f(width, height);  glTexCoord2f (1.0, 1.0);
    glVertex2f(width, 0);       glTexCoord2f (1.0, 0.0);
    glVertex2f(0, 0);           glTexCoord2f (0.0, 0.0);
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mode)
        glTranslated(0, 0, -25);

    glColor3d(1, 1, 0);

    glPushMatrix();
    glRotated(t * 25, 0, 1, 0);
    glRotated(sin(t * 5) * 25, 1, 0, 0);

    glPushMatrix();
    glTranslated(-1, 1.2, -6);
    glRotated(t * 100, .5, .5, 0);
    glTranslated(-5, 0, 0);
    glutSolidIcosahedron();
    glPopMatrix();

    glColor3d(0, 1, 0);

    for (unsigned i = 0; i < 9; ++i)
    {
        glPushMatrix();
        glTranslated(-1, 1.2, -6);
        glRotated((t * 100) + (40 * i), .5, .5, 0);
        glTranslated(5, 0, 0);
        glutSolidIcosahedron();
        glPopMatrix();
    }

    double pre = 100.0;
    glColor3d(-((double)(((int)(t * (int)pre)) % (int)pre)) / pre, ((double)(((int)(t * (int)pre)) + ((int)pre / 2) % (int)pre)) / pre, ((double)(((int)(t * (int)pre)) % (int)pre)) / pre);

    glPushMatrix();

    glTranslated(-1, 1.2, -6);
    glRotated(pow((((int)(t * 100) % 100) - 50) / 1.5, 2) / 10.0, 1, 0, 0);
    glutSolidIcosahedron();
    glPopMatrix();

    glColor3d(1, 0, 1);

    glPushMatrix();
    glRotated(t * 150, -.5, .5, 0);
    glTranslated(-1, -1, -6);
    glRotated(t * 100, 1, 0, 0);
    glRotated(t * 10,  0, 1, 0);
    glutSolidTeapot(pow((((int)(t * 100) % 100) - 50) / 10.0, 2) / 10.0);
    glPopMatrix();

    glColor3d(0, 0, 1);

    glPushMatrix();
    glTranslated(0, -.75, 6);
    glutSolidSphere(1, 64, 64);
    glPopMatrix();

    glColor3d(1, 0, 0);

    glPushMatrix();
    glTranslated(0, -.75, 6);
    glRotated(t * 100, 1, 0, 1);
    glTranslated(-pow((((int)(t * 100) % 100) - 50) / 10.0, 2) / 10.0, 0, 0);
    glutSolidCube(1.5);
    glPopMatrix();

    glColor3d(0, 1, 0);

    glPushMatrix();
    glTranslated(-5, 0, 5);
    glRotated(t * 100, 1, 0, 1);
    glutSolidTorus(pow((((int)((t + .5) * 100) % 100) - 50) / 10.0, 2) / 10.0 + .1, pow((((int)(t * 100) % 100) - 50) / 10.0, 2) / 10.0 + .1, 64, 64);
    glPopMatrix();

    glColor3d(0, 1, 1);

    const double shear[] = {
        1, sin(t), 0, 0, // ky
        cos(t), 1, 0, 0, // kx
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    glPushMatrix();
    glTranslated(5, 0, 5);
    glMultMatrixd(shear);
    glRotated(t * 100, sin(t), cos(t), 0);
    glutSolidDodecahedron();
    glPopMatrix();

    const double othershear[] = {
        1, cos(t), 0, 0, // ky
        cos(t), 1, 0, 0, // kx
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    glPushMatrix();
    glMultMatrixd(othershear);
    for (unsigned i = 0; i < 9; ++i)
    {
        glPushMatrix();
        glScaled(.25, .25, .25);
        glTranslated(-20, 0, -20);
        glRotated((t * 100) + (40 * i), .5, .5, 0);
        glTranslated(7 + (sin(t * 2) * 2), 0, 0);
        glutSolidDodecahedron();
        glPopMatrix();
    }
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
    errcheck();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '7':
            mode = !mode;
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("imateapot......wheeeee");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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

    loadTexture(&testtex, BGW, BGH);

    glutMainLoop();

    return EXIT_SUCCESS;
}
