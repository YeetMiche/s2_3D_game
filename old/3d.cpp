#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include <iostream>

using namespace std;

#define M_PI 3.14159
#define FPS_LIMIT 60

const int window_x = 300;
const int window_y = 200;
const int render_scale = 3;

int fps_limit_start;
int fps = 0;
int frame = 0;
int fps_counter_start = 0;

int scaled_x = window_x * render_scale;
int scaled_y = window_y * render_scale;

int FOV = 200;

typedef struct {
    int x,y,z;
    float a;
    int l;
}player; player P;

typedef struct{
    int x,y,z;
    float a = 0;
}obj; obj Cube;

void draw_3D(){
    
    int wx[4], wy[4], wz[] = {0,0,10,10};


    wx[0] = Cube.x * cos(Cube.a) - Cube.y * sin(Cube.a);
    wx[1] = Cube.x * cos(Cube.a + M_PI / 2) - Cube.y * sin(Cube.a);
    wx[2] = Cube.x * cos(Cube.a + M_PI) - Cube.y * sin(Cube.a);
    wx[3] = Cube.x * cos(Cube.a + M_PI * 1.5) - Cube.y * sin(Cube.a);

    wy[0] = Cube.y * cos(Cube.a) + Cube.x * sin(Cube.a);
    wy[1] = Cube.y * cos(Cube.a + M_PI / 2) + Cube.x * sin(Cube.a);
    wy[2] = Cube.y * cos(Cube.a + M_PI) + Cube.x * sin(Cube.a);
    wy[3] = Cube.y * cos(Cube.a + M_PI * 1.5) + Cube.x * sin(Cube.a);

    wx[0] = wx[0] * FOV / wy[0] + window_x / 2; wy[0] = wz[0] * FOV / wy[0] + window_y / 2;
    wx[1] = wx[1] * FOV / wy[1] + window_x / 2; wy[1] = wz[1] * FOV / wy[1] + window_y / 2;
    wx[2] = wx[2] * FOV / wy[2] + window_x / 2; wy[2] = wz[2] * FOV / wy[2] + window_y / 2;
    wx[3] = wx[3] * FOV / wy[3] + window_x / 2; wy[3] = wz[3] * FOV / wy[3] + window_y / 2;
    
    glBegin(GL_POINTS);
        glVertex2i(wx[0], wy[0]);
        glVertex2i(wx[1], wy[1]);
        glVertex2i(wx[2], wy[2]);
        glVertex2i(wx[3], wy[3]);
    glEnd();


}

void display(){
    fps_limit_start = clock();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Cube.a += 0.05;

    draw_3D();


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
    glutReshapeWindow(scaled_x, scaled_y);
}

void init(){
    glClearColor(0.12,0.12,0.12,0);
    gluOrtho2D(0,window_x, window_y, 0);

    P.x = 70; P.y = -110; P.z = 20; P.a = 0; P.l = 0;

    Cube.x = 10; Cube.y = 10; Cube.z = 10;
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_x, window_y);
    glutCreateWindow("3D Game");
    glPointSize(render_scale);
    glutDisplayFunc(display);

    init();
    
    glutMainLoop();

    return 0;
}