#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define FPS_LIMIT 60


const int render_scale = 4;
int window_x = 480;
int window_y = 270;

int fps_limit_start;
int fps = 0;
int frame = 0;
int fps_counter_start = 0;

int scaled_x = window_x * render_scale;
int scaled_y = window_y * render_scale;

int FOV = 200;

#include "3dengine_classes.hpp"
#include "world_classes.hpp"
#include "3dengine.hpp"
#include "controlls.hpp"
#include "world.hpp"

void display(){
	fps_limit_start = clock();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	move_player();
	draw_outline();

	for (int s = 0; s< S.size(); s++){
		sort_walls(s);
	}

	sort_sectors();

	for (int s = 0; s < S.size(); s++){
		for (int w = S[s].ws; w<=S[s].we; w++){
			draw_wall(W[w].x1, W[w].y1, W[w].x2, W[w].y2, S[s].z1, S[s].z2, S[s]);
		}
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

void init() {
	glClearColor(0.12,0.12,0.12,0);
	gluOrtho2D(0,window_x, window_y, 0);
	glutWarpPointer(window_x / 2, window_y / 2);

	P.x = 70; P.y = -110; P.z = 20; P.a = 0; P.l = 0;
	import_walls();
	import_sectors();
	create_textures();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    window_x = glutGet(GLUT_SCREEN_WIDTH) / render_scale;
	window_y = glutGet(GLUT_SCREEN_HEIGHT) / render_scale;
	glutInitWindowSize(window_x,window_y);
    glutCreateWindow("Doom Game");
	glutFullScreen();
    glPointSize(render_scale);
	glutDisplayFunc(display);
    glutKeyboardUpFunc(buttons_up);
	glutKeyboardFunc(buttons_down);
	glutPassiveMotionFunc(mouse_func);
	init();
    
    glutMainLoop();
	
	return 0;
}

