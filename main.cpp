#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>

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



typedef struct {
	int w,a,s,d; //Move
	int up,down,left,right; //Look
	int lctrl,space; //Height
	int pfov, mfov; //POV change
}ButtonKeys; ButtonKeys Keys;

typedef struct {
	int x, y, z;
	float a;
	int l;

}player; player P;

typedef struct {
	int x1,y1;
	int x2,y2;
}walls; walls W[30];

typedef struct {
	int ws, we;
	int z1, z2;
	int d;
	vector<int> surf;
	int surface;
}sectors; sectors S[100];

typedef struct {
	int r,g,b;
}RGB;

#include "controlls.hpp"

class Texture{
	private:
	FILE* texture_file;
	
	public:
	vector<RGB> colors; 
	int ht,vt;
	Texture(string file_name, int height, int width){
		ifstream texture_file(file_name);
		ht = height;
		vt = width;

		if (texture_file.is_open()){
			RGB color;
			char comma;
			while (texture_file >> color.r >> comma >> color.g >> comma >> color.b >> comma){
				colors.push_back(color);
			}
			texture_file.close();
		}	
	}
};

Texture red_bricks("./textures/REDBRICKS.bmp", 32,32);

void import_walls(){
    FILE* wall_file = fopen("walls.txt", "r");
    if (wall_file != NULL){
        int i = 0;
        while (feof(wall_file) == 0){
            fscanf(wall_file, "%d,%d,%d,%d\n", &W[i].x1, &W[i].y1, &W[i].x2, &W[i].y2);
            i++;
        }
    }
    else{
        printf("\n\n Error while trying to open wall file!\n\n");
        exit(-1);
    }
}

void import_sectors(){
    FILE* sector_file = fopen("sectors.txt", "r");
    if (sector_file != NULL){
        int i = 0;
        while (feof(sector_file) == 0){
            fscanf(sector_file, "%d,%d,%d,%d\n", &S[i].ws, &S[i].we, &S[i].z1, &S[i].z2);
            i++;
        }
    }
    else{
        printf("\n\n Error while trying to open wall file!\n\n");
        exit(-1);
    }
}

float distance(int x1, int x2, int y1, int y2){
	float d = sqrt(pow(x2 - x1, 2) + pow(y2-y1, 2));
	return d;
}

void sort_walls(int s){
	for (int i = S[s].ws; i < S[s].we - 1; i++){
		for (int n = S[s].ws; n < S[s].we - 1 - i; n++){
			if (distance((W[n].x2 + W[n].x1)/2, P.x, (W[n].y2 + W[n].y1)/2, P.y) < distance((W[n+1].x2 + W[n+1].x1)/2, P.x, (W[n+1].y2 + W[n+1].y1)/2, P.y)) {
				walls swp = W[n + 1]; W[n + 1] = W[n]; W[n] = swp;
			}
		}
	}
}

void clip_behind_player(int *x1, int *y1, int *z1, int x2, int y2, int z2) {
	float da = *y1;
	float db = y2;
	float d = da - db; if (d == 0) { d = 1; }
	float s = da / (da - db);
	*x1 = *x1 + s * (x2 - (*x1));
	*y1 = *y1 + s * (y2 - (*y1)); if (*y1 == 0) { *y1 = 1; }
	*z1 = *z1 + s * (z2 - (*z1));
}

void draw_texture(Texture texture){
	glBegin(GL_POINTS);
	for (int x = 0; x<texture.ht; x++){
		for (int y = 0; y<texture.vt; y++){
			int pixel = x + y*texture.vt;
			glColor3ub(red_bricks.colors[pixel].r,red_bricks.colors[pixel].g ,red_bricks.colors[pixel].b);
			glVertex2i(x,y);
		}
	}
	glEnd();
}

void fill_wall(int x1, int x2, int b1, int b2, int t1, int t2, int co) {

	float dyb = b2 - b1;
	float dyt = t2 - t1;
	float dx = x2 - x1; if (dx == 0) { dx = 1;}
	float xs = x1;
	float ustep, vstep;
	float ui = 0, vi = 0;

	ustep = red_bricks.vt * 2 / (x2 - x1 + 0.000000001);

	if (x1 < 0) { ui -= x1*ustep; x1 = 0; }
	if (x2 < 0) { x2 = 0; }
	if (x1 > window_x) { x1 = window_x; }
	if (x2 > window_x) { x2 = window_x; }

	glBegin(GL_POINTS);
	for (int x = x1; x < x2; x++) {
		int y1 = dyb * (x - xs + 0.5) / dx + b1;
		int y2 = dyt * (x - xs + 0.5) / dx + t1;

		vi = 0;
		vstep = red_bricks.ht * 2 / (y2-y1 + 0.000000001);
		
		if (y1 < 0) { vi -= y1*vstep; y1 = 0; }
		if (y2 < 0) { y2 = 0; }
		if (y1 > window_y) { y1 = window_y; }
		if (y2 > window_y) { y2 = window_y; }
		
		for (int y = y1; y < y2; y++) {
			if (vi > 32) {vi -= 32;}
			if (ui > 32) {ui -= 32;}
			int pixel = (int)vi * 32 + (int)ui;
			if (pixel >= 32*32) {pixel = 32*32 - 1;}
			if (pixel < 0) {pixel = 0;}
 			glColor3ub(red_bricks.colors[pixel].r - co,red_bricks.colors[pixel].g - co,red_bricks.colors[pixel].b - co);
			glVertex2i(x,y);
			vi += vstep;
		}
		ui += ustep;
	}
	glEnd();
}

void draw_wall(int x, int y, int u, int v, int z1, int z2) {
	int wx[4], wy[4], wz[4]; 
	float CS = cos(P.a / 180 * M_PI), SN = sin(P.a / 180 * M_PI);
	int swp;

	int sx[4], sy[4];

	for (int loop = 0; loop < 2; loop++) {

		int x1 = x - P.x, y1 = y - P.y;
		int x2 = u - P.x, y2 = v - P.y;

		

		if (loop == 1) { swp = x1; x1 = x2; x2 = swp; swp = y1; y1 = y2; y2 = swp; }

		wx[0] = x1 * CS - y1 * SN;
		wx[1] = x2 * CS - y2 * SN;
		wx[2] = wx[0];
		wx[3] = wx[1];

		wy[0] = y1 * CS + x1 * SN;
		wy[1] = y2 * CS + x2 * SN;
		wy[2] = wy[0];
		wy[3] = wy[1];

		wz[0] = z1 - P.z + ((P.l * wy[0]) / 32);
		wz[1] = z1 - P.z + ((P.l * wy[1]) / 32);
		wz[2] = z2 - P.z + ((P.l * wy[0]) / 32);
		wz[3] = z2 - P.z + ((P.l * wy[1]) / 32);

		if (wy[0] < 1 && wy[1] < 1) { return; }
		if (wy[0] < 1) {
			clip_behind_player(&wx[0], &wy[0], &wz[0], wx[1], wy[1], wz[1]);
			clip_behind_player(&wx[2], &wy[2], &wz[2], wx[3], wy[3], wz[3]);

		}

		if (wy[1] < 1) {
			clip_behind_player(&wx[1], &wy[1], &wz[1], wx[0], wy[0], wz[0]);
			clip_behind_player(&wx[3], &wy[3], &wz[3], wx[2], wy[2], wz[2]);

		}

		sx[0] = wx[0] * FOV / wy[0] + window_x / 2; sy[0] = wz[0] * FOV / wy[0] + window_y / 2;
		sx[1] = wx[1] * FOV / wy[1] + window_x / 2; sy[1] = wz[1] * FOV / wy[1] + window_y / 2;
		sx[2] = wx[2] * FOV / wy[2] + window_x / 2; sy[2] = wz[2] * FOV / wy[2] + window_y / 2;
		sx[3] = wx[3] * FOV / wy[3] + window_x / 2; sy[3] = wz[3] * FOV / wy[3] + window_y / 2;

		int color_offset = atan2((u - x), (v - y + 0.00001)) * 7;

		fill_wall(sx[0], sx[1], sy[0], sy[1], sy[2], sy[3], color_offset);
	}
}

void draw_image_3D(std::string filename, int x, int y, int z) {
	int ix, iy, iz;
	float scale;
	float CS = cos(P.a / 180 * M_PI), SN = sin(P.a / 180 * M_PI);
	
	x = x - P.x;
	y = y - P.y;

	ix = x * CS - y * SN;
	iy = y * CS + x * SN;
	iz = z - P.z + ((P.l * iy) / 32);

	if (iy < 1) { return; }

	ix = ix * FOV / iy + window_x / 2;
	iy = iz * FOV / iy + window_y / 2;

	scale = (FOV * 3) / (sqrt(pow(P.x - x, 2) + pow(P.y - y, 2))) * render_scale;

	//draw_image_offset(filename,ix, iy, scale);

}

void display(){
	fps_limit_start = clock();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	move_player();

	for (int s = 0; s<100; s++){
		sort_walls(s);
	}

	draw_texture(red_bricks);

	cout << W[S[0].ws].x1 << "," << W[S[0].we].x2 << endl;

	for (int s = 0; s<100; s++){
	for (int w = S[s].ws; w<=S[s].we; w++){
			draw_wall(W[w].x1, W[w].y1, W[w].x2, W[w].y2, S[s].z1, S[s].z2);
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

