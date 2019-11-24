#include<stdio.h>
#include<stdlib.h>
#include <GL/glut.h>


static float velicina = 0.1;

static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_display(void);

int main(int argc, char** argv){

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(480, 854);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutMainLoop();

    return 0;
}

static void on_display(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //Startna linija

    glColor3f(1,0,1);
    glLineWidth(3);
    glBegin(GL_LINES);
        glVertex2f(-1,-1+0.05);
        glVertex2f(1,-1+0.05);
    glEnd();

    //Prototip igrackog karaktera

    glBegin(GL_POLYGON);
        glVertex2f(-velicina/2,-1+velicina+0.05);
        glVertex2f(-velicina/2,-1+0.05);
        glVertex2f(velicina/2,-1+0.05);
        glVertex2f(velicina/2,-1+velicina+0.05);
    glEnd();
    glutSwapBuffers();

}