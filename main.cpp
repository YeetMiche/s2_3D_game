#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define FPS_LIMIT 60

const int res = 1;
const int window_x = 300;
const int window_y = 200;
const int render_scale = 4;

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
} ButtonKeys; ButtonKeys Keys;

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
	int surf[window_x];
	int surface;
}sectors; sectors S[100];

int texture_arr[16384] = {};

void import_texture(){
	FILE* texture_file = fopen("texture.bmp", "r");
	if (texture_file != NULL){
		int i = 0;
		while(feof(texture_file) == 0){
			fscanf(texture_file, "%d,", &texture_arr[i]);
			i++;
		}
	}
	
}

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

int distance(int x1, int x2, int y1, int y2){
	int d = sqrt(pow(x2 - x1, 2) + pow(y2-y1, 2));
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

void draw_pixel(int x, int y, int r, int g, int b) {
	glColor3ub(r,g,b);
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
}

// void draw_image_offset(std::string filename, int x, int y, float scale = 1) {
// 	float ix, iy;

// 	ix = x * render_scale - 8 * scale;
// 	iy = y * render_scale - 16 * scale;

// 	draw_image(filename, ix, iy, scale);
// }

void clip_behind_player(int *x1, int *y1, int *z1, int x2, int y2, int z2) {
	float da = *y1;
	float db = y2;
	float d = da - db; if (d == 0) { d = 1; }
	float s = da / (da - db);
	*x1 = *x1 + s * (x2 - (*x1));
	*y1 = *y1 + s * (y2 - (*y1)); if (*y1 == 0) { *y1 = 1; }
	*z1 = *z1 + s * (z2 - (*z1));
}

void fill_wall(int x1, int x2, int b1, int b2, int t1, int t2, int r = 255, int g = 0, int b = 255) {

	int dyb = b2 - b1;
	int dyt = t2 - t1;
	int dx = x2 - x1; if (dx == 0) { dx = 1; }
	int xs = x1;

	if (x1 < 1) { x1 = 1; }
	if (x2 < 1) { x2 = 1; }
	if (x1 > window_x - 1) { x1 = window_x - 1; }
	if (x2 > window_x - 1) { x2 = window_x - 1; }

	for (int x = x1; x < x2; x++) {
		int y1 = dyb * (x - xs + 0.5) / dx + b1;
		int y2 = dyt * (x - xs + 0.5) / dx + t1;
		if (y1 < 0) { y1 = 0; }
		if (y2 < 0) { y2 = 0; }
		if (y1 > window_y) { y1 = window_y; }
		if (y2 > window_y) { y2 = window_y; }
		for (int y = y1; y < y2; y++) {
			draw_pixel(x, y, r, g, b);
		}
	}
}

void draw_wall(int x, int y, int u, int v, int z1, int z2) {
	int wx[4], wy[4], wz[4]; 
	float CS = cos(P.a / 180 * M_PI), SN = sin(P.a / 180 * M_PI);
	int swp;

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

		wx[0] = wx[0] * FOV / wy[0] + window_x / 2; wy[0] = wz[0] * FOV / wy[0] + window_y / 2;
		wx[1] = wx[1] * FOV / wy[1] + window_x / 2; wy[1] = wz[1] * FOV / wy[1] + window_y / 2;
		wx[2] = wx[2] * FOV / wy[2] + window_x / 2; wy[2] = wz[2] * FOV / wy[2] + window_y / 2;
		wx[3] = wx[3] * FOV / wy[3] + window_x / 2; wy[3] = wz[3] * FOV / wy[3] + window_y / 2;

		int r, g, b;
		int color_offset = atan((u - x) / (v - y + 0.00001)) * 10;

		r = abs(170 - color_offset);
		g = abs(170 - color_offset);
		b = abs(180 - color_offset);

		fill_wall(wx[0], wx[1], wy[0], wy[1], wy[2], wy[3], r, g, b);
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

void move_player() {
	if (Keys.a == 1) { P.a -= 2; if (P.a < 0) { P.a += 360; } }
	if (Keys.d == 1) { P.a += 2; if (P.a > 359) { P.a -= 360; } }
	int dx = sin(P.a/180*M_PI) * 10.0;
	int dy = cos(P.a / 180 * M_PI) * 10.0;
	if (Keys.w == 1) { P.x += dx; P.y += dy; }
	if (Keys.s == 1) { P.x -= dx; P.y -= dy; }
	if (Keys.left == 1) { P.x -= dy/2; P.y += dx/2; }
	if (Keys.right == 1) { P.x += dy/2; P.y -= dx/2; }
	if (Keys.down == 1) { P.l -= 1; }
	if (Keys.up == 1) { P.l += 1; }
	if (Keys.lctrl == 1) { P.z += 1; }
	if (Keys.space == 1) { P.z -= 1; }

}

void display(){
	fps_limit_start = clock();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	move_player();

	for (int s = 0; s<100; s++){
		sort_walls(s);
	}

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
	glutReshapeWindow(scaled_x, scaled_y);
}

void buttons_up(unsigned char key, int x, int y){
	if (key=='a'){Keys.a=0;}
	if (key=='d'){Keys.d=0;}
	if (key=='w'){Keys.w=0;}
	if (key=='s'){Keys.s=0;}

	if (key==52){Keys.left=0;}
	if (key==54){Keys.right=0;}
	if (key==56){Keys.up=0;}
	if (key==53){Keys.down=0;}

	if (key==32){Keys.space=0;}
	if (key==17){Keys.lctrl=0;}
}

void buttons_down(unsigned char key, int x, int y){
	if (key=='a'){Keys.a=1;}
	if (key=='d'){Keys.d=1;}
	if (key=='w'){Keys.w=1;}
	if (key=='s'){Keys.s=1;}

	if (key==52){Keys.left=1;}
	if (key==54){Keys.right=1;}
	if (key==56){Keys.up=1;}
	if (key==53){Keys.down=1;}

	if (key==32){Keys.space=1;}
	if (key==17){Keys.lctrl=1;}
}


void init() {
	glClearColor(0.12,0.12,0.12,0);
	gluOrtho2D(0,window_x, window_y, 0);
	
	P.x = 70; P.y = -110; P.z = 20; P.a = 0; P.l = 0;
	import_walls();
	import_sectors();
	import_texture();
}

// int old_main(int argc, char *argv[]) {
// 	init();
// 	int t = 0;
// 	while (running()) { 
// 		t += 1;
// 		move_player();

// 		for (int s = 0; s<100; s++){
// 			sort_walls(s);
// 		}

// 		for (int s = 0; s<100; s++){
// 			for (int w = S[s].ws; w<=S[s].we; w++){
// 				draw_wall(W[w].x1, W[w].y1, W[w].x2, W[w].y2, S[s].z1, S[s].z2);
// 			}
// 		}

// 		draw_image_3D("tiles/char1.bmp", 80, 10, 40);
// 		present();
// 	}
	
// 	return 0;
// }

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_x,window_y);
    glutCreateWindow("Doom Game");
    glPointSize(render_scale);
	glutDisplayFunc(display);
    glutKeyboardUpFunc(buttons_up);
	glutKeyboardFunc(buttons_down);
	
	init();
    
    glutMainLoop();
	
	return 0;
}

