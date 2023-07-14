#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <GL/freeglut.h>

using namespace std;

#define FPS_LIMIT 60


const int render_scale = 4;
int window_x = 480;
int window_y = 270;

int fps_limit_start;
int fps = 0;
int frame = 0;
int fps_counter_start = 0;
int gameFrame = 0;

int scaled_x = window_x * render_scale;
int scaled_y = window_y * render_scale;

int FOV = 200;

#include "3dengine_classes.hpp"
#include "world_classes.hpp"
#include "3dengine.hpp"
#include "controlls.hpp"
#include "world.hpp"
#include "2dengine.hpp"
#include "game_logic.hpp"

void display(){
	fps_limit_start = clock();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	move_player();
	screen_shake();

	calculate_wall_distance();

	for (int s = 0; s < S.size(); s++){
		if (S[s].isFloor == false){
			sort_walls(s);
		}
	}

	sort_sectors();
	sort_objects();

	draw_enviroment();
	draw_world_floor();
	for (int s = 0; s < S.size(); s++){
		for (int w = S[s].ws; w<=S[s].we; w++){
			if (S[s].isFloor == false){
				draw_wall(W[w].x1, W[w].y1, W[w].x2, W[w].y2, S[s].z1, S[s].z2, S[s]);
			} else {
				draw_plane(S[s]);
			}
			for (int o = 0; o < Obj.size(); o++){
				if (distance(Obj[o].x, P.x, Obj[o].y, P.y) < W[w].distance){
					int id = Obj[o].textureID;
					draw_texture_3D(texture_list[id], Obj[o].x, Obj[o].y, Obj[o].z, Obj[o].scale);
				}
			}


		}
	}


	game.update();

	draw_weapon();
	draw_crosshair(window_x/2, window_y/2, 2);

	while ((fps_limit_start + 1000/FPS_LIMIT) - clock() > 0){
		;
	}

	frame += 1;
	if (clock() - fps_counter_start > 1000){
		fps = frame;
		frame = 0;
		fps_counter_start = clock();

	}
	gameFrame += 1;

	glutPostRedisplay();
	glutSwapBuffers();
}	

void init() {
	glClearColor(0.12,0.12,0.12,0);
	gluOrtho2D(0,window_x, window_y, 0);
	glutWarpPointer(window_x / 2, window_y / 2);

	P.x = 170; P.y = -110; P.z = 20; P.a = 0; P.l = 0;
	import_walls();
	import_sectors();
	import_objects();
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
	glutMotionFunc(mouse_func);
	glutMouseFunc(click_mouse_func);
	glutSpecialFunc(specialKeys);
	init();
    
    glutMainLoop();
	
	return 0;
}

