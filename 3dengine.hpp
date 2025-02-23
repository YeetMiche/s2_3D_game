#ifndef THREED_ENGINE_H_
#define THREED_ENGINE_H_

#define _USE_MATH_DEFINES

#include <GL/glut.h>
#include <vector>
#include <fstream>
#include <math.h>
#include "world_classes.hpp"
#include "world.hpp"
#include "2dengine.hpp"

using namespace std;


void clip_behind_player(int *x1, int *y1, int *z1, int x2, int y2, int z2) {
	float da = *y1;
	float db = y2;
	float d = da - db; if (d == 0) { d = 1; }
	float s = da / (da - db);
	*x1 = *x1 + s * (x2 - (*x1));
	*y1 = *y1 + s * (y2 - (*y1)); if (*y1 == 0) { *y1 = 1; }
	*z1 = *z1 + s * (z2 - (*z1));
}

void fill_wall(int x1, int x2, int b1, int b2, int t1, int t2, int co, Sector sector) {

	float dyb = b2 - b1;
	float dyt = t2 - t1;
	float dx = x2 - x1; if (dx == 0) { dx = 1;}
	float xs = x1;
	float ustep, vstep;
	float ui = 0, vi = 0;

	Texture texture = texture_list[sector.textureID];

	ustep = texture.vt * sector.uscale / (x2 - x1 + 0.000000001);

	if (x1 < 0) { ui -= x1*ustep; x1 = 0; }
	if (x2 < 0) { x2 = 0; }
	if (x1 > window_x) { x1 = window_x; }
	if (x2 > window_x) { x2 = window_x; }

	glBegin(GL_POINTS);
	for (int x = x1; x < x2; x++) {
		int y1 = dyb * (x - xs + 0.5) / dx + b1;
		int y2 = dyt * (x - xs + 0.5) / dx + t1;

		vi = 0;
		vstep = texture.ht * sector.vscale / (y2-y1 + 0.000000001);
		
		if (y1 < 0) { vi -= y1*vstep; y1 = 0; }
		if (y2 < 0) { y2 = 0; }
		if (y1 > window_y) { y1 = window_y; }
		if (y2 > window_y) { y2 = window_y; }

		for (int y = y1; y < y2; y++) {
			if (vi > texture.ht) {vi -= texture.ht;}
			if (ui > texture.vt) {ui -= texture.vt;}

			int pixel = (int)vi * texture.ht + (int)ui;
			if (pixel >= pow(texture.ht,2)) {pixel = pow(texture.ht,2) - 1;}
			if (pixel < 0) {pixel = 0;}
			
			RGB color;

			if (texture.colors[pixel].g == 255 && texture.colors[pixel].b == 255){
				;
			} else {
				color.r = texture.colors[pixel].r - co;
				if (color.r < 0) {color.r = 0;}
				if (color.r > 255) {color.r = 255;}
				color.g = texture.colors[pixel].g - co;
				if (color.g < 0) {color.g = 0;}
				if (color.g > 255) {color.g = 255;}
				color.b = texture.colors[pixel].b - co;
				if (color.b < 0) {color.b = 0;}
				if (color.b > 255) {color.b = 255;}
			
				glColor3ub(color.r, color.g, color.b);
				glVertex2i(x,y);
			}
			vi += vstep;
		}
		ui += ustep;
	}
	glEnd();
}

void draw_plane(Sector sector, bool top) {
	int wx[4], wy[4], wz[4]; 
	float CS = cos(P.a / 180 * M_PI), SN = sin(P.a / 180 * M_PI);
	int swp;
	int x,y,u,v;
	int z1 = sector.z1;
	int z2 = sector.z2;

	vector<int> tsx, tsy;
	vector<int> bsx, bsy;
	for (int i = sector.ws; i <= sector.we; i++){


		x = W[i].x1;
		y = W[i].y1;
		u = W[i].x2;
		v = W[i].y2;


		int x1 = x - P.x, y1 = y - P.y;
		int x2 = u - P.x, y2 = v - P.y;


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

		tsx.push_back(wx[0] * FOV / wy[0] + window_x / 2); tsy.push_back(wz[0] * FOV / wy[0] + window_y / 2);
		bsx.push_back(wx[2] * FOV / wy[2] + window_x / 2); bsy.push_back(wz[2] * FOV / wy[2] + window_y / 2);
	}

	int r = color_list[sector.textureID].r;
	int g = color_list[sector.textureID].g;
	int b = color_list[sector.textureID].b;
	if (top){
		glBegin(GL_POLYGON);
			glColor3ub(r,g,b);
			for (int i = 0; i < tsx.size(); i++){
				glVertex2i(tsx[i], tsy[i]);
			}
		glEnd();
	} else {
		glBegin(GL_POLYGON);
		glColor3ub(100,100,100);
		for (int i = 0; i < bsx.size(); i++){
			glVertex2i(bsx[i], bsy[i]);
		}
		glEnd();
	}

}



void draw_wall(int x, int y, int u, int v, int z1, int z2, Sector sector) {
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

		fill_wall(sx[0], sx[1], sy[0], sy[1], sy[2], sy[3], color_offset, sector);

		
	}
}

void draw_texture_3D(Texture texture, int x, int y, int z, float scale = 1) {
	int ix, iy, iz;
	float CS = cos(P.a / 180 * M_PI), SN = sin(P.a / 180 * M_PI);
	
	scale = FOV * render_scale * scale / distance(x,P.x,y,P.y);
    
    x = x - P.x;
	y = y - P.y;
    z = z - P.z;

	ix = x * CS - y * SN;
	iy = y * CS + x * SN;
	iz = z - P.z + ((P.l * iy) / 32);
	if (iy < 1) { return; }
	if (sqrt(pow(x,2) + pow(y,2)) < 25){ return; }

	ix = ix * FOV / iy + window_x / 2;
	iy = iz * FOV / iy + window_y / 2;

	draw_texture(texture, ix, iy, scale);
}

void draw_text_3D(string str, int x, int y, int z, RGB color, float scale = 1) {
	int tx, ty, tz;
	float CS = cos(P.a / 180 * M_PI), SN = sin(P.a / 180 * M_PI);

	scale = FOV * render_scale * scale / distance(x,P.x,y,P.y);

    x = x - P.x;
	y = y - P.y;
    z = z - P.z;

	tx = x * CS - y * SN;
	ty = y * CS + x * SN;
	tz = z - P.z + ((P.l * ty) / 32);
	if (ty < 1) { return; }
	if (sqrt(pow(x,2) + pow(y,2)) < 25){ return; }

	tx = tx * FOV / ty + window_x / 2;
	ty = tz * FOV / ty + window_y / 2;

	tx -= get_text_sizex(str,scale) / 2;

	draw_text(tx,ty,str,color.r,color.g,color.b,scale);
}

void draw_world_floor(){
	glBegin(GL_QUADS);
		glColor3ub(10,20,0);
		glVertex2i(0, window_y / 2 + P.l);
		glColor3ub(30,80,10);
		glVertex2i(0,window_y);
		glColor3ub(30,80,10);
		glVertex2i(window_x, window_y);
		glColor3ub(10,20,0);
		glVertex2i(window_x, window_y/2 + P.l);
	glEnd();
}

void draw_enviroment(){
	glBegin(GL_QUADS);
		glColor3ub(10,20,50);
		glVertex2i(0,0);
		glVertex2i(window_x,0);
		glColor3ub(120,50,30);
		glVertex2i(window_x, window_y);
		glVertex2i(0,window_y);
	glEnd();
}

#endif /*THREED_ENGINE_H_*/