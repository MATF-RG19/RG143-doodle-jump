#include<stdio.h>
#include<stdlib.h>
#include <GL/glut.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 10
#define velicina 0.1
#define jump_limit 0.2
#define levo 1
#define gore 2
#define desno 3
#define dole 4 
#define pomeraj 0.04

static int jump_up = 0;
static int jump_down = 0;

static float xc=velicina/2;
static float yc=velicina+0.05;

static void on_keyboard(unsigned char key,int x,int y);
static void on_timer(int value);
static void on_display(void);
static void on_reshape(int height, int width);
static void skok(int value);

static void igrac();
static void teren();

int smer;
static float x_curr = 5;
static float z_curr = 3;
static float y_curr = 2;

int main(int argc, char** argv){

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(480, 854);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Doodle jump");

    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
  

    glClearColor(0.6,0.6,0.8, 0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}

static void on_reshape(int width, int height){
    
    glViewport(0, 0, width, height);

    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,(float) width / height, 1, 100);
}

static void on_display(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    gluLookAt(10,4,4,0,0,2,0,1,0);

    //iscrtavanje axisa radi lakse orijentacije u prostoru
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
    glEnd();

    glBegin(GL_LINES);
        glVertex3f(4,0,0);
        glVertex3f(4,0,4);
    glEnd();

    glColor3f(0,1,0);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
    glEnd();

    teren();   
    igrac();

    glutSwapBuffers();
}

//funkcija koja iscrtava igraca; zasad je igrac samo mala kockica
static void igrac(){
    glColor3f(0.3,0.3,0.3);
    glColor3f(0,0,0);

    glPushMatrix();
        glTranslatef(x_curr,y_curr,z_curr);
        glScalef(0.3,0.3,0.3);
        glutSolidCube(1);
    glPopMatrix();
}

static void on_keyboard(unsigned char key, int x, int y){

        switch (key)
        {
        case 27:
            exit(0);
            break;

        case 'w':
            smer=gore;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;
        case 'a':
            smer=levo;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;
        case 'd':
            smer=desno;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;
        case 's':
            smer=dole;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;

        case ' ':
            if(!jump_up) 
                jump_up = 1;
                // glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;    
        
        default:
            break;
        }
}

// Funkcija koja iscrtava donju ivicu terena
static void teren(){
    int ivica = 5;

    glColor3f(0.5,0.4,0.6);
    glPushMatrix();
        glTranslatef(ivica,-ivica,ivica/2);
        glScalef(1,1,0.4); 
        glutSolidCube(ivica*2);
       
    glPopMatrix();
}



static void on_timer(int value){

   glutPostRedisplay();
   switch (smer){
       case levo:
            z_curr+=pomeraj;
            break;
       case desno:
            z_curr-=pomeraj;
            break;
        case gore:
            x_curr-=pomeraj;
            break;
        case dole:
            x_curr+=pomeraj;
   };
   
   glPushMatrix();
        glTranslatef(x_curr,y_curr,z_curr);
        glScalef(1,1,0.4);
        glutSolidCube(10);
    glPopMatrix();
    

}

static void skok(int value){

}