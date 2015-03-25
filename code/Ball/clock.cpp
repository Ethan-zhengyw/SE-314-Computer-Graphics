/**
 * from http://blog.csdn.net/sssogs/article/details/8223493
 */
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define PI 3.1415926

float h,m,s;

void Draw()
{
    int i;
    float R,TR,h_Angle,m_Angle,s_Angle,count,h_Length,m_Length,s_Length;
    R=0.5;
    TR=R-0.05;
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    for (i=0; i<100; i++)
    {
        glVertex2f(R*cos(2*PI/100*i),R*sin(2*PI/100*i));
    }
    glEnd();
    glLineWidth(2);
    for (i=0; i<12; i++)
    {
        glBegin(GL_LINES);
        glVertex2f(TR*sin(2*PI/12*i),TR*cos(2*PI/12*i));
        glVertex2f(R*sin(2*PI/12*i),R*cos(2*PI/12*i));
        glEnd();
    }

    glLineWidth(1);

    h_Length=0.2;
    m_Length=0.3;
    s_Length=0.4;
    count=60;
    s_Angle=s/count;
    count*=60;
    m_Angle=(m*60+s)/count;
    count*=12;
    h_Angle=(h*60*60+m*60+s)/count;
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(s_Length*sin(2*PI*s_Angle),s_Length*cos(2*PI*s_Angle));
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(h_Length*sin(2*PI*h_Angle),h_Length*cos(2*PI*h_Angle));
    glEnd();
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(0.0f,0.0f);
    glVertex2f(m_Length*sin(2*PI*m_Angle),m_Length*cos(2*PI*m_Angle));
    glEnd();
    glLineWidth(1);

    glBegin(GL_POLYGON);
    for (i=0; i<100; i++)
    {
        glVertex2f(0.03*cos(2*PI/100*i),0.03*sin(2*PI/100*i));
    }
    glEnd();
    glutSwapBuffers();
}

void Update()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    h=timeinfo->tm_hour;
    m=timeinfo->tm_min;
    s=timeinfo->tm_sec;
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("HelloOpenGL");
    glutIdleFunc(&Update);
    glutDisplayFunc(&Draw);
    glutMainLoop();
    return 0;
}
