#ifndef TWOD_ENGINE_H_
#define TWOD_ENGINE_H_

#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <string>
#include <math.h>
#include "world_classes.hpp"

using namespace std;


void draw_text(int x, int y, string text, int r, int g, int b){
	glColor3ub(r,g,b);
	glRasterPos2i(x,y);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char*>(text.c_str()));
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
	if (P.shooting){
		if (gameFrame%10 - 5 < 0){
			draw_texture(texture_list[8], window_x/4 * 3, window_y - 100, 1);
		} else {
			draw_texture(texture_list[9], window_x/4 * 3, window_y - 100, 1);
		}

	} else {
		draw_texture(texture_list[7], window_x/4 * 3, window_y - 100, 1);
	}
}

#endif /*TWOD_ENGINE_H_*/