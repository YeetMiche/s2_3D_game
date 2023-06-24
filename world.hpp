#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

class Wall{
    public:
    int x1,y1;
    int x2,y2;

    Wall(int _x1, int _y1, int _x2, int _y2){
        x1 = _x1;
        y1 = _y1;
        x2 = _x2;
        y2 = _y2;
    }
};

vector<Wall> W;

class Sector{
    public:
    int ws, we;
    int z1,z2;
    int d;
    vector<int> surf;
    int surface;

    Sector(int _ws, int _we, int _z1, int _z2){
        ws = _ws;
        we = _we;
        z1 = _z1;
        z2 = _z2;
    }
};

vector<Sector> S;

void import_walls(){
    ifstream wall_file("walls.txt");

    if (wall_file.is_open()){
        char comma;
        int x1,y1,x2,y2;
        while (wall_file >> x1 >> comma >> y1 >> comma >> x2 >> comma >> y2){
            W.push_back(Wall(x1,y1,x2,y2));
        } 
    }
}

void import_sectors(){
    ifstream sector_file("sectors.txt");

    if (sector_file.is_open()){
        char comma;
        int ws,we,z1,z2;
        while (sector_file >> ws >> comma >> we >> comma >> z1 >> comma >> z2){
            S.push_back(Sector(ws,we,z1,z2));
        } 
    }
}

float distance(int x1, int x2, int y1, int y2){
	float d = sqrt(pow(x2 - x1, 2) + pow(y2-y1, 2));
	return d;
}

void sort_walls(int s){
	for (int i = 0; i < S[s].we; i++){
		for (int n = S[s].ws; n < S[s].we - i; n++){
			if (distance((W[n].x2 + W[n].x1)/2, P.x, (W[n].y2 + W[n].y1)/2, P.y) < distance((W[n+1].x2 + W[n+1].x1)/2, P.x, (W[n+1].y2 + W[n+1].y1)/2, P.y)) {
				Wall swp = W[n + 1]; W[n + 1] = W[n]; W[n] = swp;
			}
		}
	}
}

int sector_distance(Sector sector){
    float x = 0, y = 0;

    for (int i = sector.ws; i < sector.we; i++){
        x += W[i].x1;
        y += W[i].y1;
    }

    x/=(sector.we - sector.ws);
    y/=(sector.we - sector.ws);

    x -= P.x;
    y -= P.y;

    return sqrt(x*x + y*y);
}



void sort_sectors(){
    for (int i = 0; i < S.size(); i++){
        for (int n = 0; n < S.size() - i - 1; n++){
            cout << "sorting sector " << n << endl;
            if (sector_distance(S[n]) < sector_distance(S[n+1])){
                Sector swp = S[n+1];
                S[n+1] = S[n];
                S[n] = swp;
            }
        }
    }
}

#endif