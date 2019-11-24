#include<stdio.h>
#include<stdlib.h>
#include <GL/glut.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20
#define velicina 0.1
#define jump_limit 0.2

static int jump_up = 0;
static int jump_down = 0;


static float xc=velicina/2;
static float yc=velicina+0.05;

static void on_keyboard(unsigned char key,int x,int y);
static void on_timer(int value);
static void on_display(void);

int main(int argc, char** argv){

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(480, 854);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);

    glutMainLoop();

    return 0;
}

static void on_display(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Ogranicavajuce linije sa strana

    glLineWidth(3);
    glColor3f(0,0,1);
    glBegin(GL_LINES);
        glVertex2f(-1+0.05,1);
        glVertex2f(-1+0.05,-1);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(1-0.05,1);
        glVertex2f(1-0.05,-1);
    glEnd();

    //Startna linija

    glColor3f(1,0,1);
    glBegin(GL_LINES);
        glVertex2f(-1,-1+0.05);
        glVertex2f(1,-1+0.05);
    glEnd();

    //Prototip igrackog karaktera

    glBegin(GL_POLYGON);
        glVertex2f(-xc,-1+yc);
        glVertex2f(-xc,-1+0.05);
        glVertex2f(xc,-1+0.05);
        glVertex2f(xc,-1+yc);
    glEnd();
    glutSwapBuffers();

}

static void on_keyboard(unsigned char key, int x, int y){

        switch (key)
        {
        case 27:
            exit(0);

        case ' ':
            if(!jump_up) 
                jump_up = 1;
                glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;    
        
        default:
            break;
        }
}

//prototip funkcije skakanja

static void on_timer(int value){

    if(value!=TIMER_ID){
        return;
    }    
    float dy = 0.003;
    
    if(jump_up==1){
        yc+=dy;
    }
    glutPostRedisplay();

    if(jump_up==1){
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
    }

}