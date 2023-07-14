#ifndef GAME_MATH_H_
#define GAME_MATH_H_

#define _USE_MATH_DEFINES

#include <math.h>

int calculate_angle(int x1, int y1, int x2, int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;

    float angle_rad = atan2(dy,dx);
    float angle_deg = angle_rad * 180 / M_PI;
    return angle_deg;

}

float distance(int x1, int x2, int y1, int y2){
	float d = sqrt(pow(x2 - x1, 2) + pow(y2-y1, 2));
	return d;
}

#endif /*GAME_MATH_H_*/