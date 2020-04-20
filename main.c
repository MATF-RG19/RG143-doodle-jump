#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Makroi za rad funkcija

#define TIMER_ID 0
#define TIMER_INTERVAL 20
#define velicina 0.1
#define jump_limit 0.2
#define levo 1
#define gore 2
#define desno 3
#define dole 4 
#define pomeraj 0.03

// Globalne promenljive

static time_t t;

// Promenljive koje definisu da li je igrac u skoku/padu

static int jump_up = 0;
static int jump_down = 0;

// Niz koji definise kretanje levo/desno
static int niz[2] = {0,0};

// promenljive koje definisu prepreke
// TODO - dodati jos prepreka
static float prep[50];
static float y_prep[50];

// Promenljiva koja definise maksimalni dohvat pri skoku
static int dohvat = 48;

// funkcije za rad sa GLUT bibliotekom

static void on_keyboard(unsigned char key,int x,int y);
static void on_timer(int value);
static void on_display(void);
static void on_reshape(int height, int width);
static void skok(int value);
static void keyboard_up(unsigned char key, int x, int y);

// funkcije koje iscrtavaju igraca/teren/prepreke
static void igrac();
static void teren();
static void prepreke();
static void popuni();

// koordinate pocetnog polozaja igraca i brzina kretanja

static float x_curr = 4;
static float z_curr = 2;
static float y_curr = 1;
static float dy = 0.005;


int main(int argc, char** argv){

    popuni();
    // for(int i=0;i<50;i++)
    //     printf("i:%d:%f\n",i,y_prep[i]);
    glutInit(&argc, argv);
    glutInitWindowSize(480, 854);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Doodle jump");

    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutKeyboardUpFunc(keyboard_up);


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


    glLineWidth(3);
    glColor3f(0,0,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    gluLookAt(10,y_curr,2,0,y_curr,2,0,1,0);
    
    //iscrtavanje axisa radi lakse orijentacije u prostoru
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
    glEnd();
        glColor3f(0,0,1);
    glBegin(GL_LINES);
        glVertex3f(0,0,-2);
        glVertex3f(0,0,6);
    glEnd();
    glColor3f(0,1,0);
    glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
    glEnd();

    glColor3f(1,0,1);

    //Prototip igrackog karaktera i terena
    teren(); 
    igrac();
    prepreke();
    

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
    switch(key){
        case 27:
            exit(0);
            break;
        case 'a':
            //smer=levo;
            niz[0]=1;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;
        case 'd':
            //smer=desno;
            niz[1]=1;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;

        case ' ':
            // skok
             if(!jump_up && !jump_down) 
                {
                    jump_up = 1;

                glutTimerFunc(TIMER_INTERVAL,skok,TIMER_ID);
                } 
            break;    

        default:
            break;
        }
}



static void keyboard_up(unsigned char key, int x, int y){
switch(key){
        case 27:
            exit(0);
            break;
        case 'a':
            //smer=levo;
            niz[0]=0;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;
        case 'd':
            //smer=desno;
            niz[1]=0;
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
    if (niz[0])
        z_curr+=pomeraj;
    if (niz[1])
        z_curr-=pomeraj;


    if(jump_down)
        for(int i=48;i>2;i--){
            if (y_curr-0.15>=y_prep[i]){
                dohvat = i;
                break;
            }
        }
        //printf("ovo je dohvat:%d, a jumpd: %d, a y:%f\n",dohvat, jump_down,y_curr);
        
        if ((z_curr+0.15<prep[dohvat] || z_curr-0.15>prep[dohvat]+0.5) 
            && (jump_down==0 && jump_up==0) && dohvat!=48){
                jump_down = 1;
                //printf("usao0: dohvat:%d jumpd:%d jumpup:%d\n",dohvat, jump_down,jump_up);
                glutTimerFunc(TIMER_INTERVAL,skok,TIMER_ID);
       }
    
   

   glPushMatrix();
        glTranslatef(x_curr,y_curr,z_curr);
        glScalef(1,1,0.4);
        glutSolidCube(10);
    glPopMatrix();

   
    glutPostRedisplay();

}

static void popuni(){
    srand((unsigned) time(&t));
    
    for (int i=0;i<50;i++){
        float x = (float)rand()/(float)(RAND_MAX/4);
            prep[i]=x;
            y_prep[i]=i/4.0;
            }
};



static void prepreke(){
    glLineWidth(4);
    glColor3f(1,0,0);
    
    // Pocetna linija
    glBegin(GL_LINES);
        glVertex3f(4,y_prep[2],prep[2]-50);
        glVertex3f(4,y_prep[2],prep[2]+30);
    glEnd();
    
    for (int i=3;i<50;i++){
    glBegin(GL_LINES);
        glVertex3f(4,y_prep[i],prep[i]);
        glVertex3f(4,y_prep[i],prep[i]+0.5);
    glEnd();}
}

static void skok(int value){
    if(jump_up == 1){
        dy += 0.005;
        y_curr += dy;

        if (dy>=0.1){
            jump_up = 0;
            dy = 0.005;
            jump_down = 1;
        }

    }

    for(int i=48;i>=0;i--){
            if (y_curr-0.15>=y_prep[i] && y_curr+0.15<y_prep[i+1]){
                dohvat = i;
                break;}
    }
    
    if (jump_down == 1){
        if (dy<0.1)
            dy += 0.005;

        y_curr -= dy;
        
        if ((z_curr+0.15>prep[dohvat] && z_curr-0.15<prep[dohvat]+0.5)&& y_curr<y_prep[dohvat]+0.15){
            jump_down = 0;
            dy=0.005;
            //printf("usao1 a dohvat:%d, jumpd:%d, jumpup:%d\n",dohvat,jump_down,jump_up);  
        }
        else if(y_curr-0.16<y_prep[dohvat] && dohvat>2){
            //printf("usao2 a dohvat:%d, jumpd:%d, jumpup:%f\n",dohvat,jump_down,y_curr);               
            dohvat--;
        }
        else if (y_curr-0.16<=y_prep[2]){
            jump_down = 0;
            dy=0.005;
            //printf("usao3 a dohvat:%d, jumpd:%d, jumpup:%d\n",dohvat,jump_down,jump_up);  
            y_curr = y_prep[2]+0.15;
        }

    }

    glPushMatrix();
        glTranslatef(x_curr,y_curr,z_curr);
        glScalef(1,1,0.4);
        glutSolidCube(10);
    glPopMatrix();
    glutPostRedisplay();

    

    if(jump_up==1 || jump_down==1){
        glutTimerFunc(TIMER_INTERVAL,skok,TIMER_ID);
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
    }

} 