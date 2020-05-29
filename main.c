#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "image.h"


// Imena fajlova sa teksturama
#define FILENAME0 "noc.bmp"
#define FILENAME1 "tekstura.bmp"
#define FILENAME2 "neon.bmp"
#define FILENAME3 "end.bmp"

// Identifikatori tekstura
static GLuint names[4];


// Makroi za boje

#define NEON (glColor3f(0.5,0.07,1))
#define RED (glColor3f(1, 0, 0))
#define GRAY (glColor3f(0.5, 0.5, 0.5))
#define LIME (glColor3f(0.1, 1 , 0))
#define AQUA (glColor3f(0, 0.8 , 1))
#define LIGHT (glColor3f(1, 1 , 1))
#define SAND (glColor3f(0.97, 0.70 , 0.50))
#define BLUE (glColor3f(0, 0 , 0.90))

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
static int started = 0;

// Promenljiva koja meri skor
static float score = 0;

// Promenljive koje definisu da li je igrac u skoku/padu

static int jump_up = 0;
static int jump_down = 0;

// Niz koji definise kretanje levo/desno
static int niz[2] = {0,0};

// promenljive koje definisu prepreke
static float prep[1000];
static float y_prep[1000];

// Promenljiva koja definise maksimalni dohvat pri skoku
static int dohvat = 998;

// funkcije za rad sa GLUT bibliotekom


static void start();
static void on_keyboard(unsigned char key,int x,int y);
static void on_timer(int value);
static void on_display(void);
static void on_reshape(int height, int width);
static void skok(int value);
static void keyboard_up(unsigned char key, int x, int y);
static void end();

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

    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.5, 0.07, 1, 1 };
    GLfloat light_specular[] = {0.5, 0.07, 1, 1 };

    GLfloat ambient_coeffs[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat diffuse_coeffs[] = { 0.8, 0.8, 0.8, 1 };
    GLfloat specular_coeffs[] = { 0.5, 0.5, 0,5, 1 };
    GLfloat shininess = 1.3;   

    popuni();
    glutInit(&argc, argv);
    glutInitWindowSize(480, 854);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Doodle jump");

       // Objekat koji predstavlja teskturu ucitanu iz fajla
         Image * image;

         // Ukljucuju se teksture.
         glEnable(GL_TEXTURE_2D);

         glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

          //Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz fajla
         image = image_init(0, 0);

         // Generisu se identifikatori tekstura
         glGenTextures(4, names);

         // Kreira se prva tekstura. 
         image_read(image, FILENAME0);

        glBindTexture(GL_TEXTURE_2D, names[0]);
         glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
         glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
         glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

        
          image_read(image, FILENAME1);

        glBindTexture(GL_TEXTURE_2D, names[1]);
        glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
        
        image_read(image, FILENAME2);

        glBindTexture(GL_TEXTURE_2D, names[2]);
         glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
         glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
         glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
        
        image_read(image, FILENAME3);

        glBindTexture(GL_TEXTURE_2D, names[3]);
        glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);


                 // Iskljucujemo aktivnu teksturu 
          glBindTexture(GL_TEXTURE_2D, 0);

         // Unistava se objekat za citanje tekstura iz fajla. 
          image_done(image);


    glutDisplayFunc(start);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutKeyboardUpFunc(keyboard_up);


    glClearColor(0.6,0.6,0.8, 0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);   		

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    



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

    GLfloat light_position[] = {5,score,10,1};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


   // gluLookAt(10,5,2,0,score,2,0,1,0);
    gluLookAt(10,y_curr,2,0,score,2,0,1,0); // prava

    glDisable(GL_LIGHTING);
    glPushMatrix();
        LIGHT;
        glRasterPos3f(0,  score+5, 5);
    
        char score_display[50] = "SCORE: ";
        char score_value[50];
    
        sprintf(score_value, " %.f ", score*10);
        strcat(score_display, score_value);
        int len = (int)strlen(score_display);
        int i;
        for (i = 0; i < len; i++){
            glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, score_display[i]);
        }
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glPushMatrix();

        glScalef(3,20,3);
        glRotatef(270,1,0,0);
        glRotatef(90,0,0,1);
        glTranslatef(0 ,7, 3);
  
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, names[0]);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-10, 2, -5);
			
			glTexCoord2f(1, 0);
			glVertex3f(10, 2, -5);
			
			glTexCoord2f(1, 6);
			glVertex3f(10, 2, 5);
			
			glTexCoord2f(0, 6);
			glVertex3f(-10, 2, 5);
		glEnd();
	
        glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    
   

    //Prototip igrackog karaktera
    igrac();
    prepreke();

    glutSwapBuffers();
}

static void igrac(){

// MODEL HELIKOPTERA
    
    RED;
    glPushMatrix();
        glRotatef(0.5,0,1,0);
        glTranslatef(x_curr,y_curr+0.4,z_curr);
        glScalef(0.8,0.005,0.8);
        glutWireSphere(0.5,50,50);
    glPopMatrix();
    NEON;
    glPushMatrix();
        glTranslatef(x_curr,y_curr-0.1,z_curr-0.1);
        glScalef(0.2,0.05,0.05);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(x_curr,y_curr-0.1,z_curr+0.1);
        glScalef(0.2,0.05,0.05);
        glutSolidCube(1);
    glPopMatrix();
    AQUA;
    glPushMatrix();
        glTranslatef(x_curr,y_curr+0.2,z_curr);
        glScalef(0.7,0.2,0.2);
        glutSolidSphere(1,10,10);
    glPopMatrix();

}

// Funkcije sa tastature 

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
            if (!started){
                glutDisplayFunc(on_display);
                glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                started = 1;
            }
             if(!jump_up && !jump_down){
                jump_up = 1;
                glutTimerFunc(TIMER_INTERVAL,skok,TIMER_ID);
                } 
            break;
        case 'r':
            score = 0;
            started = 0;
            x_curr =  4;
            z_curr = 1;
            y_curr = 1;
            glutDisplayFunc(on_display);
            glutPostRedisplay();
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


// funckija pomeranja

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
        for(int i=999;i>2;i--){
            if (y_curr-0.15>=y_prep[i]){
                dohvat = i;
                break;
            }
        }
        
        if ((z_curr+0.15<prep[dohvat] || z_curr-0.15>prep[dohvat]+0.5) 
            && (jump_down==0 && jump_up==0) && dohvat!=999){
                jump_down = 1;
                glutTimerFunc(TIMER_INTERVAL,skok,TIMER_ID);
       }
    
   

   
    glutPostRedisplay();

}


// Stvaranje nasumicnih nizova za prepreke

static void popuni(){
    srand((unsigned) time(&t));
    
    for (int i=0;i<1000;i++){
        float x = (float)rand()/(float)(RAND_MAX/4);
            prep[i]=x;
            y_prep[i]=i/4.0;
            }
};



static void prepreke(){
    glLineWidth(20);
    RED;
    glDisable(GL_LIGHTING);

    // Pocetna linija
    glBegin(GL_LINES);
        glVertex3f(4,y_prep[2],prep[2]-50);
        glVertex3f(4,y_prep[2],prep[2]+30);
    glEnd();
    
    glLineWidth(4);
    for (int i=3;i<1000;i++){
        if(i==100)
            AQUA;
        if(i==200)
            GRAY;
        if(i==300)
            NEON;
        if(i==400)
            LIGHT;
        if(i==500)
            RED;
        if(i==600)
            LIME;
        if(i==700)
            SAND;
        if(i==800)
            BLUE;    
        

    
        glBegin(GL_LINES);
            glVertex3f(4,y_prep[i],prep[i]);
            glVertex3f(4,y_prep[i],prep[i]+0.5);
        glEnd();
        }

    glEnable(GL_LIGHTING);
}


static void skok(int value){

    if(jump_up == 1){
        dy += 0.004;
        y_curr += dy;

        if (dy>=0.1){
            jump_up = 0;
            dy = 0.005;
            jump_down = 1;
        }

        if(y_curr>score)
            score = y_curr;
    }

    // for(int i=48;i>=0;i--){
    //         if (y_curr-0.15>=y_prep[i] && y_curr+0.15<y_prep[i+1]){
    //             dohvat = i;
    //             break;}
    // }
    
    if (jump_down == 1){
        if (dy<0.1)
            dy += 0.002;

        y_curr -= dy;
        
        if ((z_curr+0.15>prep[dohvat] && z_curr-0.15<prep[dohvat]+0.5)&& y_curr<y_prep[dohvat]+0.15){
            jump_down = 0;
            dy=0.005;
        }
        else if(y_curr-0.16<y_prep[dohvat] && dohvat>2){             
            dohvat--;
        }
        else if (y_curr-0.16<=y_prep[2]){
            jump_down = 0;
            dy=0.005;  
            y_curr = y_prep[2]+0.15;
        }

    }

    if (score>y_curr+5){
        glutDisplayFunc(end);
    }

    // glPushMatrix();
    //     glTranslatef(x_curr,y_curr,z_curr);
    //     glScalef(1,1,0.4);
    //     glutSolidCube(10);
    // glPopMatrix();
    // glutPostRedisplay();

    

    if(jump_up==1 || jump_down==1){
        glutTimerFunc(TIMER_INTERVAL,skok,TIMER_ID);
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
    }

} 

// funkcija koja vodi na pocetni ekran
static void start(){


    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 4, 0,
              0, 0, 0,
              1, 0, 0);
    
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, names[2]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-2, 0, -2);
			
			glTexCoord2f(0, 1);
			glVertex3f(2, 0, -2);
			
			glTexCoord2f(1, 1);
			glVertex3f(2, 0, 2);
			
			glTexCoord2f(1, 0);
			glVertex3f(-2, 0, 2);
		glEnd();
        
	glDisable(GL_TEXTURE_2D);
    

    glutSwapBuffers();
}


// funkcija koja izlazi iz igre(game over ekran)
static void end(){


    started = 0;

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 4, 0,
              0, 0, 0,
              1, 0, 0);
    
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, names[3]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-2, 0, -2);
			
			glTexCoord2f(0, 1);
			glVertex3f(2, 0, -2);
			
			glTexCoord2f(1, 1);
			glVertex3f(2, 0, 2);
			
			glTexCoord2f(1, 0);
			glVertex3f(-2, 0, 2);
		glEnd();
        
	glDisable(GL_TEXTURE_2D);

    glDisable(GL_LIGHTING);
    glPushMatrix();
        NEON;
        glRasterPos3f(-0.08, 0.2, -0.25);
        char score_display[50] = " ";
        char score_value[50];
        
        sprintf(score_value, " %.f ", score*10);
        strcat(score_display, score_value);

        int len = (int)strlen(score_display);
        int i;
        for ( i = 0; i < len; i++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_display[i]);
        }
    glPopMatrix();
    glEnable(GL_LIGHTING);
    

    glutSwapBuffers();
}