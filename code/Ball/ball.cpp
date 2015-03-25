#include <GL/glut.h>
#include <iostream>
using namespace std;

float rotate_x = 0, rotate_y = 0, scale = 1;
float trans_x = 0, trans_y = 0;

void drawGLScene()		// Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    glRotatef(rotate_x, 1, 0, 0);
    glRotatef(rotate_y, 0, 1, 0);
    glScalef(scale, scale, 1);
    glTranslatef(trans_x, trans_y, 0.0f);

    /*
    //      lef  rig  bot  top  near far
    // glOrtho(10.0, 100.0, 0.0, 0.0, 0, 0);  // 待看，三维的情况比较好理解
    // glTranslatef(0.5f,0.0f,0.0f);			// Move Right
    glViewport(0, 200, 300, 200);  // 跟glutInitWindowSize中的参数有关,定义了在窗口中的位置和大小
    glScalef(0.5, 0.5, 1.0);
    glBegin(GL_TRIANGLES);	       // Drawing Using Triangles
    glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f( 0.0f, 1.0f, 0.0f); // Top
    glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left
    glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
    glEnd();					   // Finished Drawing

    glViewport(300, 200, 300, 200);  // 跟glutInitWindowSize中的参数有关,定义了在窗口中的位置和大小
    glScalef(1.0, -1.0, 1.0);
    glBegin(GL_TRIANGLES);	       // Drawing Using Triangles
    glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f( 0.0f, 1.0f, 0.0f); // Top
    glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left
    glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
    glEnd();					   // Finished Drawing

    glScalef(2.0, 2.0, 1.0);
    glViewport(0, 0, 300, 200);  // 跟glutInitWindowSize中的参数有关,定义了在窗口中的位置和大小
    glBegin(GL_QUADS);				// Draw A Quad
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);		// Top Left
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f( 0.5f, 0.5f, 0.0f);		// Top Right
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f( 0.5f,-0.5f, 0.0f);		// Bottom Right
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f,-0.5f, 0.0f);		// Bottom Left
    glEnd();
*/
    glViewport(0, 0, 600, 300);  // 跟glutInitWindowSize中的参数有关,定义了在窗口中的位置和大小
    glRotatef(45, 0, 0, 1);
    glBegin(GL_QUADS);				// Draw A Quad
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);		// Top Left
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f( 0.5f, 0.5f, 0.0f);		// Top Right
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f( 0.5f,-0.5f, 0.0f);		// Bottom Right
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f,-0.5f, 0.0f);		// Bottom Left
    glEnd();

    glutSwapBuffers();
}

void roate(unsigned char key, int x, int y)
{
    switch(key) {
        case 'a': rotate_y -= 10; break;
        case 'd': rotate_y += 10; break;
        case 'w': rotate_x -= 10; break;
        case 's': rotate_x += 10; break;
        case 'u': scale += 0.1; break;
        case 'j': scale -= 0.1; break;
        case 't': trans_y += 0.1; break;
        case 'g': trans_y -= 0.1; break;
        case 'h': trans_x += 0.1; break;
        case 'f': trans_x -= 0.1; break;
        case 'z': exit(0);  break;
    }
    // cout << degree_x << '!' << degree_y << endl;
    glutPostRedisplay();
} 

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Hello OpenGL!");
    glutDisplayFunc(drawGLScene);
    glutKeyboardFunc(roate);
    glutMainLoop();
    return 0;
}
