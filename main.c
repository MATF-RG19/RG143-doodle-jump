#include<stdio.h>
#include<stdlib.h>
#include <GL/glut.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 10
#define jump_limit 0.2
#define levo 1
#define gore 2
#define desno 3
#define dole 4 
#define pomeraj 0.04

static int jump_up = 0;
static int jump_down = 0;


static void on_keyboard(unsigned char key,int x,int y);
static void on_timer(int value);
static void on_display(void);
static void on_reshape(int height, int width);
static void on_release(unsigned char key,int x,int y);
static void skok(int value);

static void igrac();
static void teren();

int smer;
static float x_curr = 5;
static float z_curr = 2 ;
static float y_curr = 2;

GLdouble mat2=4;
GLdouble mat3=2;
static int pomeranje[2]={0,0}; 

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
    
    
    gluLookAt(10,mat2,2,0,mat3,2,0,1,0);

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

    // gluLookAt(10,mat2,4,0,0,2,0,1,0);

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


        case 'a':
            pomeranje[0]=1;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;
        case 'd':
            pomeranje[1]=1;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;

        case ' ':
            if(!jump_up) 
                jump_up = 1;
                glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;    
        
        default:
            break;
        }
}

static void on_release(unsigned char key, int x, int y){
    
    switch (key){
    case 'a':
        pomeranje[0] -= 1;
        glutPostRedisplay();
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
        break;
    case 'd':
        pomeranje[1] -= 1;
        glutPostRedisplay();
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
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
    if(value!=TIMER_ID){
        return;
    }

   glutPostRedisplay();
   if(pomeranje[1])
       z_curr-=pomeraj;
   
   if(pomeranje[0])
       z_curr+=pomeraj;

//    while (jump_up>0){
//        glutTimerFunc(20,skok,1);
//    }
   
   glPushMatrix();
        glTranslatef(x_curr,y_curr,z_curr);
        glScalef(1,1,0.4);
        glutSolidCube(10);
    glPopMatrix();
    

}

static void skok(int value){
            if(value!=1){
                return;
            }

            mat2+=pomeraj;
            mat3+=pomeraj;
            y_curr+=pomeraj;
            jump_up-=pomeraj;
            glutPostRedisplay();

}