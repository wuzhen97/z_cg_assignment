/*****************************************************************************
        FILE : submit.c (Assignment 1)
        NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
        Student Information
        Student ID: 
        Student Name: 
*****************************************************************************/
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <GL/glut.h>

// using namespace std;

#define PI 3.14159265358979323846 // pi
// --------------------------------开关标志-------------------------------//
bool naturalLight_flag = true;
bool ceilingLamp_flag = false;
bool fans_flag = false;
bool robot_flag = false;
bool robot_eye = false;
int fanangle = 0;

// ------------------------------机器人初始化-----------------------------//
int robotAng = -60;
float robotX = -2.0;
float robotY = -7.0;
float robotZ = -5.0;
float robot_scale = 1.0;
float robotFoot1 = 1.5;
float robotFoot2 = 1.5;

// --------------------------------常用颜色-------------------------------//
GLfloat matWall[] = {0.37647, 0.83529, 0.98431};
GLfloat matBlack[] = {0, 0, 0, 1};
GLfloat matWhite[] = {1, 1, 1, 1};
GLfloat matPink[] = {1, 0.8, 0.2, 1};
GLfloat matBrown[] = {1, 0.6, 0.3, 1};
GLfloat matYellowGreen[] = {0.5, 1, 0.5, 1};
GLfloat matRed[] = {1, 0, 0, 1};
GLfloat matGreen[] = {0, 1, 0, 1};
GLfloat matBlue[] = {0, 0, 1, 1};
GLfloat matYellow[] = {1, 1, 0, 1};
GLfloat matShininess[] = {50};

// --------------------------------光源设置-------------------------------//
struct LightAttribute
{
    GLfloat position[4];
    GLfloat direction[3];
    GLfloat cutoff;
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
};

// 自然光
LightAttribute light0 = {
    {0, 9, 10, 1},
    {0, -1, -1},
    45,
    {0.2, 0.2, 0.2, 1.0},
    {0.4, 0.4, 0.4, 1.0},
    {1.0, 1.0, 1.0, 1.0}};

// 吊灯
LightAttribute light1 = {
    {0, 10, 0, 1},
    {0, -1, 0},
    50,
    {0.5, 0.5, 0.5, 0},
    {0.5, 0.5, 0.5, 0},
    {0.5, 0.5, 0.5, 0},
};

// --------------------------------辅助函数-------------------------------//
void setLight(bool flag, GLenum lightID, LightAttribute &light)
{
    glLightfv(lightID, GL_POSITION, light.position);
    glLightfv(lightID, GL_SPOT_DIRECTION, light.direction);
    glLightf(lightID, GL_SPOT_CUTOFF, light.cutoff);
    glLightfv(lightID, GL_AMBIENT, light.ambient);
    glLightfv(lightID, GL_DIFFUSE, light.diffuse);
    glLightfv(lightID, GL_SPECULAR, light.specular);

    if (flag)
        glEnable(lightID);
    else
        glDisable(lightID);
}

void drawQuads(GLfloat x1, GLfloat y1, GLfloat z1,
               GLfloat x2, GLfloat y2, GLfloat z2,
               GLfloat x3, GLfloat y3, GLfloat z3,
               GLfloat x4, GLfloat y4, GLfloat z4)
{
    glBegin(GL_QUADS); // top wall
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x4, y4, z4);
    glEnd();
}

void drawWall()
{
    glColor3f(0.37647, 0.83529, 0.98431);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWall);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWall);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);
    // left
    drawQuads(-10, 10, -20,
              -10, 10, 0,
              -10, -10, 0,
              -10, -10, -20);
    // right
    drawQuads(10, -10, -20,
              10, -10, 0,
              10, 10, 0,
              10, 10, -20);
    // back
    drawQuads(10, 10, -20,
              -10, 10, -20,
              -10, -10, -20,
              10, -10, -20);
    // top
    drawQuads(10, 10, -20,
              10, 10, 0,
              -10, 10, 0,
              -10, 10, -20);
    // bottom
    glColor3f(0.75294, 0.75294, 0.75294);
    drawQuads(-10, -10, -20,
              -10, -10, 0,
              10, -10, 0,
              10, -10, -20);
}

void drawBed()
{

    glColor3f(1, 0.8, 0.2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    // 床头
    glPushMatrix();
    glTranslatef(-9.5, -6, -16);
    glScalef(1, 8, 8);
    glutSolidCube(1);
    glPopMatrix();

    // 床尾
    glPushMatrix();
    glTranslatef(4.5, -8.5, -16);
    glScalef(1, 3, 8);
    glutSolidCube(1);
    glPopMatrix();

    //床板
    glPushMatrix();
    glTranslatef(-2, -7, -16);
    glScalef(14, 1, 8);
    glutSolidCube(1);
    glPopMatrix();

    //床垫
    glColor3f(1, 0.8, 0.8);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matPink);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matPink);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    glPushMatrix();
    glTranslatef(-2, -6, -16);
    glScalef(14, 1, 8);
    glutSolidCube(1);
    glPopMatrix();
}

void drawCloset()
{
    glColor3f(1, 0.6, 0.3);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    glPushMatrix();
    glTranslatef(-8, -8, -10);
    glScalef(4, 4, 4);
    glutSolidCube(1);
    glPopMatrix();
}

void drawDesk()
{
    glColor3f(1, 0.6, 0.3);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matBrown);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    glPushMatrix();
    glTranslatef(9, -4, -10);
    glScalef(2, 0.5, 20);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(9.75, -7, -10);
    glScalef(0.5, 6, 20);
    glutSolidCube(1);
    glPopMatrix();
}

void drawTeapot()
{
    glColor3f(1, 1, 1);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    glPushMatrix();
    glTranslatef(-8, -5.5, -10);
    glRotatef(-135, 0, 1, 0);
    glutSolidTeapot(0.5);
    glPopMatrix();
}

void drawRobot()
{
    glColor3f(0.5, 1, 0.5);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matYellowGreen);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matYellowGreen);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);

    glPushMatrix();
    glTranslatef(robotX, robotY, robotZ);            // motion
    glRotatef(robotAng, 0, 1, 0);                    // rotation
    glScalef(robot_scale, robot_scale, robot_scale); // scale

    glPushMatrix();
    glRotatef(90, 1, 0, 0); // Body
    gluCylinder(quadratic, 1, 1, 1.5, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glRotatef(90, 1, 0, 0); // Hand
    glTranslatef(0, 1.2, 0.3);
    gluCylinder(quadratic, 0.2, 0.2, 1, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glRotatef(90, 1, 0, 0); // Hand
    glTranslatef(0, -1.2, 0.3);
    gluCylinder(quadratic, 0.2, 0.2, 1, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glRotatef(90, 1, 0, 0); // Foot1
    glTranslatef(0, 0.5, robotFoot1);
    gluCylinder(quadratic, 0.2, 0.2, 1, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glRotatef(90, 1, 0, 0); // Foot2
    glTranslatef(0, -0.5, robotFoot2);
    gluCylinder(quadratic, 0.2, 0.2, 1, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 1.2, 0.7); // Antenna
    glRotatef(90, 3, 1.5, 0);
    gluCylinder(quadratic, 0.1, 0.1, 0.5, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 1.2, -0.7); // Antenna
    glRotatef(90, 3, 1.5, 0);
    gluCylinder(quadratic, 0.1, 0.1, 0.5, 16, 16);
    glPopMatrix();
    glPushMatrix();
    glutSolidSphere(1, 16, 16); // Head
    glPopMatrix();
    glPushMatrix();
    if (robot_eye)
        glColor3f(1, 0, 0);
    else
        glColor3f(0, 0, 0);
    glTranslatef(0.75, 0, -0.5);
    gluCylinder(quadratic, 0.2, 0.2, 1, 16, 16); // eye
    glPopMatrix();

    glPopMatrix();
}

void drawFans()
{

    glPushMatrix();
    glTranslatef(0, 11, -10);
    glColor3f(1, 1, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, matYellow);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matYellow);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matBlack);

    // 挂杆
    glPushMatrix();
    glTranslatef(0, -1.5, 0);
    glScalef(0.6, 3, 0.6);
    glutSolidCube(1);
    glPopMatrix();

    glRotatef(fanangle, 0, 1, 0); // 旋转
    if (ceilingLamp_flag)
        glMaterialfv(GL_FRONT, GL_EMISSION, matYellow); // 灯

    // 电机
    glPushMatrix();
    glTranslatef(0, -3, 0);
    glScalef(1, 0.6, 1);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    // 三片扇叶
    glPushMatrix();
    glTranslatef(0, -3, 0);
    glRotatef(0, 0, 1, 0);
    glTranslatef(0, 0, -3);
    glScalef(1.5, 0.35, 4.5);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -3, 0);
    glRotatef(120, 0, 1, 0);
    glTranslatef(0, 0, -3);
    glScalef(1.5, 0.35, 4.5);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -3, 0);
    glRotatef(240, 0, 1, 0);
    glTranslatef(0, 0, -3);
    glScalef(1.5, 0.35, 4.5);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}

// --------------------------------下面为作业模板-------------------------------//
void init(void) // All Setup For OpenGL Goes Here
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void display(void) // Here's Where We Do All The Drawing
{
    // TODO:
    // Place light source here
    setLight(naturalLight_flag, GL_LIGHT0, light0);
    setLight(ceilingLamp_flag, GL_LIGHT1, light1);

    // TODO:
    // Draw walls and objects here
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawWall();
    drawBed();
    drawDesk();
    drawCloset();
    drawTeapot();
    drawRobot();
    drawFans();

    // TODO:
    // Add animation here
    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(65, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    gluLookAt(0.0, 0.0, 15.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{
    switch (key)
    {
    case '\033': // press 'esc' to quit
        exit(0);
        break;
        // TODO:
        // Add keyboard control here
    case '1':
        if (!naturalLight_flag && ceilingLamp_flag)
            ceilingLamp_flag = !ceilingLamp_flag;
        naturalLight_flag = !naturalLight_flag;
        break;
    case '2':
        if (!naturalLight_flag)
            ceilingLamp_flag = !ceilingLamp_flag;
        break;
    case '3':
        fans_flag = !fans_flag;
        break;
    case '4':
        robot_flag = !robot_flag;
        break;
    case 'w':
        if (robot_flag == true)
        {
            robotZ -= 0.1 * sin(robotAng / 180.0 * PI);
            robotX += 0.1 * cos(robotAng / 180.0 * PI);
        }
        break;
    case 's':
        if (robot_flag == true)
        {
            robotZ += 0.1 * sin(robotAng / 180.0 * PI);
            robotX -= 0.1 * cos(robotAng / 180.0 * PI);
        }
        break;
    case 'q':
        if (robot_flag == true)
            robotAng = (robotAng + 2) % 360;
        break;
    case '+':
        if (robot_flag == true && robot_scale < 1.5)
            robot_scale += 0.1;
        break;
    case '-':
        if (robot_flag == true && robot_scale > 0.5)
            robot_scale -= 0.1;
        break;
    case '0':
        if (robot_flag == true)
            robot_scale = 1;
        break;
    case 'e':
        if (robot_flag == true)
            robotAng = (robotAng - 2) % 360;
        break;
    }
}

int count1 = 0;
void idle(void)
{
    // 机器人做运动状态
    if (robot_flag == true)
    {
        if ((++count1 % 50) > 24)
        {
            robotFoot1 = 0;
            robotFoot2 = 1.5;
            robot_eye = true;
        }
        else
        {
            robotFoot1 = 1.5;
            robotFoot2 = 0;
            robot_eye = false;
        }
    }
    else
    {
        robotFoot1 = 1.5;
        robotFoot2 = 1.5;
        robot_eye = false;
    }

    // 风扇转动
    if (fans_flag == true)
    {
        fanangle = (fanangle + 1) % 360;
    }

    Sleep(5);

    glutPostRedisplay();
}

int main(int argc, char **argv)
{

    /*Initialization of GLUT Library */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    /*Create a window with title specified */
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Assignment 1");

    init(); /*not GLUT call, initialize several parameters */

    /*Register different CALLBACK function for GLUT to response
    with different events, e.g. window sizing, mouse click or
    keyboard stroke */
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    /*Enter the GLUT event processing loop which never returns.
    it will call different registered CALLBACK according
    to different events. */
    glutMainLoop();

    return 0;
}