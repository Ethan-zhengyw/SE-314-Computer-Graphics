#include <GL/glut.h>

void drawGLScene()		// Here's Where We Do All The Drawing
{
    //Clear Screen And Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Reset The Current Modelview Matrix
    glLoadIdentity();


    glTranslatef(-0.5f,0.0f,-0.5f);	// Move 1.5 Left And 6.0 Into The Screen.
    glBegin(GL_TRIANGLES);		// Drawing Using Triangles
    glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top
	glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
	glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
    glEnd();					// Finished Drawing

    //glTranslatef(3.0f,0.0f,0.0f);			// Move Right
//    glBegin(GL_QUADS);				// Draw A Quad
//    glColor3f(1.0f, 1.0f, 0.0f);
//	glVertex3f(-1.0f, 1.0f, 0.0f);		// Top Left
//	glVertex3f( 1.0f, 1.0f, 0.0f);		// Top Right
//	glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
//	glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
//    glEnd();

    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutCreateWindow("Hello OpenGL!");
    glutDisplayFunc(drawGLScene);
    glutMainLoop();
    return 0;
}
