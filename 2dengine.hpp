#ifndef TWOD_ENGINE_H_
#define TWOD_ENGINE_H_

#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <string>
#include <math.h>
#include "world_classes.hpp"
#include "sound_engine.hpp"

using namespace std;


void draw_texture(Texture texture, int x = 0, int y = 0, float scale = 1){

	x = x - texture.ht/2*scale;
	y = y - texture.vt/2*scale;
	glPixelZoom(2,2);
	glBegin(GL_POINTS);
	for (int xi = 0; xi<texture.ht * scale; xi++){
		for (int yi = 0; yi<texture.vt * scale; yi++){
			int pixel = static_cast<int>(xi / scale) + static_cast<int>(yi / scale)*texture.vt;
			if (texture.colors[pixel].g == 255 && texture.colors[pixel].b == 255 && texture.colors[pixel].r == 0){ 
				;
			} else if (xi + x < 0 || xi + x > window_x || yi + y < 0 || yi + y > window_y){
				;
			} else {
				glColor3ub(texture.colors[pixel].r,texture.colors[pixel].g ,texture.colors[pixel].b);
				glVertex2i(xi + x,yi + y);
			}
		}
	}
	glEnd();
}

void draw_texture_no_transparency(Texture texture, int x = 0, int y = 0, float xscale = 1, float yscale = 1){
	glBegin(GL_POINTS);
	for (int xi = 0; xi<texture.ht * xscale; xi++){
		for (int yi = 0; yi<texture.vt * yscale; yi++){
			int pixel = static_cast<int>(xi / xscale) + static_cast<int>(yi / yscale)*texture.vt;
			glColor3ub(texture.colors[pixel].r,texture.colors[pixel].g ,texture.colors[pixel].b);
			glVertex2i(xi + x,yi + y);
		}
	}
	glEnd();
}

// void draw_text(int x, int y, string text, int r, int g, int b){
// 	glColor3ub(r,g,b);
// 	glRasterPos2i(x,y);
// 	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, reinterpret_cast<const unsigned char*>(text.c_str()));
// }

void draw_text(int x, int y, string text, int r, int g, int b, float scale = 1){
	scale /= 50;
	glColor3ub(r,g,b);
	glPushMatrix();
	glTranslatef(x,y,0);
	glRotatef(180,180,0,0);
	glScalef(scale,scale,1);
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, reinterpret_cast<const unsigned char*>(text.c_str()));
	glPopMatrix();
}

void draw_crosshair(int x, int y, int size) {
    glLineWidth(2.0f);
    glColor3ub(200,200,200); // White color

    glBegin(GL_LINES);

    // Vertical line
    glVertex2i(x, y - size);
    glVertex2i(x, y + size);

    // Horizontal line
    glVertex2i(x - size, y);
    glVertex2i(x + size, y);

    glEnd();
}

void draw_weapon(){
	int x = window_x / 4 * 3 - sin(gameFrame / 4) * P.sa / 30;
	int y = window_y - 80 - cos(gameFrame / 3) * P.sa / 10;

	if (P.shooting){
		if (gameFrame%10 - 5 < 0){
			draw_texture(texture_list[8], x, y, 1);
		} else {
			draw_texture(texture_list[9], x, y, 1);
		}

	} else {
		draw_texture(texture_list[7], x, y, 1);
	}
}

#endif /*TWOD_ENGINE_H_*/