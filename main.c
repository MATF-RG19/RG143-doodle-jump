#include<stdio.h>
#include<stdlib.h>
#include <GL/glut.h>

int main(int argc, char** argv){

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(480, 854);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

}