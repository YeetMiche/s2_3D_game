#ifndef CONTROLLS_H_
#define CONTROLLS_H_

#include <GL/glut.h>
#include <vector>
#include "world_classes.hpp"


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


//can't use this with second screen
void mouse_func(int x, int y){
	if (x <= 0){glutWarpPointer(window_x * render_scale, y);}
	if (x >= window_x * render_scale - 1){glutWarpPointer(0, y);}
	P.a = float(x) / (render_scale * window_x) * 360;
	glutSetCursor(GLUT_CURSOR_NONE);
}

/*can use this with second screen but the mouse movement is stuttery*/
// void mouse_func(int x, int y){
// 	int deltaX = x - window_x * render_scale/2;
// 	P.a += deltaX/10;
// 	if (deltaX != 0){glutWarpPointer(window_x * render_scale/2, window_y * render_scale/2);}
// 	glutSetCursor(GLUT_CURSOR_NONE);
// }

void click_mouse_func(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		P.shooting = true;
	} else {
		P.shooting = false;
	}
}

bool isCollision(float x, float y, float radius, float x1, float y1, float x2, float y2) {
    float dist = abs((y2 - y1) * x - (x2 - x1) * y + x2 * y1 - y2 * x1) /
                 sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));

    if (dist <= radius) {
        float minX = min(x1, x2);
        float maxX = max(x1, x2);
        float minY = min(y1, y2);
        float maxY = max(y1, y2);

        if (x >= minX - radius && x <= maxX + radius && y >= minY - radius && y <= maxY + radius) {
            return true;  // Collision detected
        }
    }

    return false;  // No collision
}



void move_player() {
	int shakeChange = 2;
	int dx = sin(P.a/180*M_PI) * 10.0;
	int dy = cos(P.a / 180 * M_PI) * 10.0;
	if (Keys.w == 1) { 
		P.x += dx; 
		P.y += dy; 

		P.sa += shakeChange;
		for (int w = 0; w < W.size(); w++){
			if (isCollision(P.x,P.y,P.cr, W[w].x1, W[w].y1, W[w].x2, W[w].y2)) {
				P.x -= dx;
				P.y -= dy;
			}
		}
		
	}

	if (Keys.s == 1) { 
		P.x -= dx; 
		P.y -= dy;

		P.sa += shakeChange;
		for (int w = 0; w < W.size(); w++){
			if (isCollision(P.x,P.y,P.cr, W[w].x1, W[w].y1, W[w].x2, W[w].y2)) {
				P.x += dx;
				P.y += dy;
			}
		}
	
	}

	if (Keys.a == 1) { 
		P.x -= dy/2; 
		P.y += dx/2; 
	
		P.sa += shakeChange;


		for (int w = 0; w < W.size(); w++){
			if (isCollision(P.x,P.y,P.cr, W[w].x1, W[w].y1, W[w].x2, W[w].y2)) {
				P.x += dy/2;
				P.y -= dx/2;
			}
		}
	}
	
	if (Keys.d == 1) { 
		P.x += dy/2; 
		P.y -= dx/2;
		
		P.sa += shakeChange;


		for (int w = 0; w < W.size(); w++){
			if (isCollision(P.x,P.y,P.cr, W[w].x1, W[w].y1, W[w].x2, W[w].y2)) {
				P.x -= dy/2;
				P.y += dx/2;
			}
		}
	}


	if (Keys.down == 1) { ; } // P.l -= 1
	if (Keys.up == 1) { ; } // P.l += 1
	if (Keys.lctrl == 1) { ; } // P.z += 1
	if (Keys.space == 1) { ; } //P.z -= 1
	if (Keys.pfov == 1) { FOV += 5; }
	if (Keys.mfov == 1) { FOV -= 5; }
}

void screen_shake(){
	P.sa -= 1.1;
	if (P.sa > 40) {P.sa = 40;}
	if (P.sa < 0) {P.sa = 0;}

	P.l = (float)sin(gameFrame/3) * P.sa /110;

}

void specialKeys(int key, int x, int y) {
    switch (key){
        case GLUT_KEY_UP:
            std::cout << "Up arrow key pressed" << std::endl;
            break;
        case GLUT_KEY_DOWN:
            std::cout << "Down arrow key pressed" << std::endl;
            break;
        case GLUT_KEY_LEFT:
            std::cout << "Left arrow key pressed" << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            std::cout << "Right arrow key pressed" << std::endl;
            break;
    }
}

#endif
