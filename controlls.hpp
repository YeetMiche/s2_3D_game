#ifndef CONTROLLS_H_
#define CONTROLLS_H_

#include <GL/glut.h>

extern player P;

typedef struct {
	int w,a,s,d; //Move
	int up,down,left,right; //Look
	int lctrl,space; //Height
	int pfov, mfov; //POV change
}buttonKeys; buttonKeys Keys;

void buttons_up(unsigned char key, int x, int y){
	if (key=='a'){Keys.a=0;}
	if (key=='d'){Keys.d=0;}
	if (key=='w'){Keys.w=0;}
	if (key=='s'){Keys.s=0;}

	if (key=='i'){Keys.up=0;}
	if (key=='k'){Keys.down=0;}

	if (key=='u'){Keys.space=0;}
	if (key=='m'){Keys.lctrl=0;}

	if (key=='r'){Keys.pfov=0;}
	if (key=='t'){Keys.mfov=0;}
}

void buttons_down(unsigned char key, int x, int y){
	if (key=='a'){Keys.a=1;}
	if (key=='d'){Keys.d=1;}
	if (key=='w'){Keys.w=1;}
	if (key=='s'){Keys.s=1;}

	if (key=='i'){Keys.up=1;}
	if (key=='k'){Keys.down=1;}

	if (key=='u'){Keys.space=1;}
	if (key=='m'){Keys.lctrl=1;}

	if (key=='r'){Keys.pfov=1;}
	if (key=='t'){Keys.mfov=1;}
}


//can't use this way with second screen
void mouse_func(int x, int y){
	if (x <= 0){glutWarpPointer(window_x * render_scale, y);}
	if (x >= window_x * render_scale - 1){glutWarpPointer(0, y);}
	P.a = float(x) / (render_scale * window_x) * 360;
	glutSetCursor(GLUT_CURSOR_NONE);
}

//can use this with second screen but the mouse movement is stuttery
// // void mouse_func(int x, int y){
// // 	int deltaX = x - window_x * render_scale/2;
// // 	P.a += deltaX/10;
// // 	if (deltaX != 0){glutWarpPointer(window_x * render_scale/2, window_y * render_scale/2);}
// // 	glutSetCursor(GLUT_CURSOR_NONE);
// // }

void move_player() {
	int dx = sin(P.a/180*M_PI) * 10.0;
	int dy = cos(P.a / 180 * M_PI) * 10.0;
	if (Keys.w == 1) { P.x += dx; P.y += dy; }
	if (Keys.s == 1) { P.x -= dx; P.y -= dy; }
	if (Keys.a == 1) { P.x -= dy/2; P.y += dx/2; }
	if (Keys.d == 1) { P.x += dy/2; P.y -= dx/2; }
	if (Keys.down == 1) { P.l -= 1; }
	if (Keys.up == 1) { P.l += 1; }
	if (Keys.lctrl == 1) { P.z += 1; }
	if (Keys.space == 1) { P.z -= 1; }
	if (Keys.pfov == 1) { FOV += 5; }
	if (Keys.mfov == 1) { FOV -= 5; }
}

#endif
