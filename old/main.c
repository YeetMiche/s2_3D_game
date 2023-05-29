#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define PI 3.14159
#define FPS_LIMIT 60
#define DEBUG_MODE 1


int window_x = 1024;
int window_y = 512;

//values for ship position
float ship_x = 100;
float ship_y = 100;
float ship_angle = 0;
int ship_size = 70;
float ship_dx, ship_dy;
char ship_angle_str[10];
char ship_x_str[10];
char ship_y_str[10];

//values for path
float ship_path_x[300];
float ship_path_y[300];
int path_length = 300;
int path_pos = 0;


//values for fps counter and limit
char fps_str[10];
int fps_limit_start;
int frame = 0;
int fps = 0;
int fps_counter_xo = 0;
int fps_counter_start = 0;

typedef struct{
     int w,a,s,d;
}ButtonKeys; ButtonKeys Keys;



void drawText(float x, float y, void *font, const char* string, float r, float g, float b){

     glColor3f(r,g,b); 
     glRasterPos2f(x, y);

     glutBitmapString(font, string);
}

void drawShip(){
     glBegin(GL_TRIANGLES);
     glColor3f(0,0.4,1);
     glVertex2i(ship_x + cos(ship_angle)*ship_size,ship_y + sin(ship_angle)*ship_size);
     glColor3f(0,0.1,0.2);   
     glVertex2i(ship_x + cos(ship_angle+2*PI/3)*ship_size,ship_y + sin(ship_angle+2*PI/3)*ship_size);
     glColor3f(0,0.1,0.2);
     glVertex2i(ship_x + cos(ship_angle-2*PI/3)*ship_size,ship_y + sin(ship_angle-2*PI/3)*ship_size);
     glEnd();
}

void drawShipPath(){

     ship_path_x[0] = ship_x;
     ship_path_y[0] = ship_y;



     for (int i = path_length-1; i >= 0; i--){
          ship_path_x[i+1] = ship_path_x[i];
          ship_path_y[i+1] = ship_path_y[i];
     }


     glLineWidth(4);
     glBegin(GL_LINE_STRIP);
     
     for (int i=0; i<path_length-1; i++) {
          glColor3f(1-0.0001*i,1-0.0001*i,1-0.0001*i);
          glVertex2f(ship_path_x[i],ship_path_y[i]);
     }
     glEnd();
     
     
}

void drawPositionText(){
     snprintf(ship_x_str, 10, "%.0f", ship_x);
     snprintf(ship_y_str, 10, "%.0f", ship_y);
     drawText(window_x - window_x*0.095,window_y - window_y*0.85, GLUT_BITMAP_TIMES_ROMAN_24, "X",1,1,1);
     drawText(window_x - window_x*0.095,window_y - window_y*0.8, GLUT_BITMAP_TIMES_ROMAN_24, "Y",1,1,1);
     drawText(window_x - window_x*0.075,window_y - window_y*0.85, GLUT_BITMAP_TIMES_ROMAN_24, ship_x_str,1,1,1);
     drawText(window_x - window_x*0.075,window_y - window_y*0.8, GLUT_BITMAP_TIMES_ROMAN_24, ship_y_str,1,1,1);
}

void display(){
     fps_limit_start = clock();

     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


     drawShipPath();
     drawShip();
     drawPositionText();


     //keyboard controll
     if (Keys.a==1) {ship_angle -= 0.1; if (ship_angle<0) {ship_angle = 2*PI;} 
          ship_dx = cos(ship_angle)*5; ship_dy = sin(ship_angle)*5;}

     if (Keys.d==1) {ship_angle += 0.1; if (ship_angle>2*PI) {ship_angle = 0;} 
          ship_dx = cos(ship_angle)*5; ship_dy = sin(ship_angle)*5;}

     if (Keys.w==1) {ship_x += ship_dx; ship_y += ship_dy;}
     if (Keys.s==1) {ship_x -= ship_dx; ship_y -= ship_dy;}

     if (ship_x > window_x) {ship_x = window_x;}
     if (ship_y > window_y) {ship_y = window_y;}
     if (ship_x < 0) {ship_x = 0;}
     if (ship_y < 0) {ship_y = 0;}
     

     //fps limit and displaying
     if (DEBUG_MODE == 1) {
          snprintf(fps_str, 10, "%d", fps);
          drawText(window_x - window_x*0.075 + fps_counter_xo, window_y - window_y*0.95, GLUT_BITMAP_TIMES_ROMAN_24, strncat(fps_str, "FPS", 3),1,1,1);
     }

     while ((fps_limit_start + 1000/FPS_LIMIT) - clock() > 0){
          ;
     }

     frame += 1;
     if (clock() - fps_counter_start > 1000){
          fps = frame;
          frame = 0;
          fps_counter_start = clock();
     }


     glutPostRedisplay();
     glutSwapBuffers();
}

void buttonsDown(unsigned char key, int x, int y){
     if (key=='a'){Keys.a=1;}
     if (key=='d'){Keys.d=1;}
     if (key=='w'){Keys.w=1;}
     if (key=='s'){Keys.s=1;}
}

void buttonsUp(unsigned char key, int x, int y){
     if (key=='a'){Keys.a=0;}
     if (key=='d'){Keys.d=0;}
     if (key=='w'){Keys.w=0;}
     if (key=='s'){Keys.s=0;}
}

void init(){
     glClearColor(0.12,0.12,0.12,0);
     gluOrtho2D(0,window_x,window_y,0);
     ship_dx = cos(ship_angle)*5; ship_dy = sin(ship_angle)*5;

     fps_counter_start = clock();

     //fill path with ship x and y position
     for (int i=0; i<path_length; i++) {
          ship_path_x[i] = ship_x;
          ship_path_y[i] = ship_y;
     }

    
}

void exitProgramm(){
     printf("Goodbye!");
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_x,window_y);
    glutCreateWindow("");
    glutDisplayFunc(display);
    glutKeyboardFunc(buttonsDown);
    glutKeyboardUpFunc(buttonsUp);
    glutCloseFunc(exitProgramm);
    init();
    
    glutMainLoop();
}



