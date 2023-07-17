#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <math.h>
#include <fstream>
#include "game_math.hpp"
using namespace std;

void import_walls(){
    ifstream wall_file("walls.txt");

    if (wall_file.is_open()){
        char comma;
        int x1,y1,x2,y2;
        while (wall_file >> x1 >> comma >> y1 >> comma >> x2 >> comma >> y2){
            W.push_back(Wall(x1,y1,x2,y2));
        } 
        wall_file.close();
    }

}

void import_sectors(){
    ifstream sector_file("sectors.txt");

    if (sector_file.is_open()){
        char comma;
        int ws,we,z1,z2;
        int texID;
        int isFloor;
        while (sector_file >> ws >> comma >> we >> comma >> z1 >> comma >> z2 >> comma >> texID >> comma >> isFloor){
            S.push_back(Sector(ws,we,z1,z2,isFloor,texID,1,1,rand()));
        } 
        sector_file.close();
    }

}

void import_objects(){
    ifstream object_file("objects.txt");

    if (object_file.is_open()){
        char comma;
        int x,y,z;
        int texID;
        float scale;
        while (object_file >> x >> comma >> y >> comma >> z >> comma >> texID >> comma >> scale){
            Obj.push_back(Object(x,y,z,texID,scale));
        }
        object_file.close();
    }

}

void calculate_wall_distance(){
    for (int i = 0; i < W.size(); i++){
        W[i].distance = distance((W[i].x2 + W[i].x1)/2, P.x, (W[i].y2 + W[i].y1)/2, P.y);
    }
}   

void sort_walls(int s){
	for (int i = 0; i < S[s].we; i++){
		for (int n = S[s].ws; n < S[s].we - i; n++){
			if (W[n].distance < W[n+1].distance) {
				Wall swp = W[n + 1]; 
                W[n + 1] = W[n]; 
                W[n] = swp;
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
    
    return sqrt(x*x + y*y) - sector.z1;
}

void sort_sectors(){
    for (int i = 0; i < S.size(); i++){
        for (int n = 0; n < S.size() - i - 1; n++){
            if (sector_distance(S[n]) < sector_distance(S[n+1])){
                Sector swp = S[n+1];
                S[n+1] = S[n];
                S[n] = swp;
            }
        }
    }
}

void sort_objects(){
    for (int i = 0; i < Obj.size(); i++){
        for (int n = 0; n < Obj.size() - i - 1; n++){
            if (distance(Obj[n].x, P.x, Obj[n].y, P.y) < distance(Obj[n+1].x, P.x, Obj[n+1].y, P.y)){
                Object swp = Obj[n+1];
                Obj[n+1] = Obj[n];
                Obj[n] = swp;
            }
        }
    }
}

#endif