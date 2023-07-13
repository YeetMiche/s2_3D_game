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

#endif /*TWOD_ENGINE_H_*/