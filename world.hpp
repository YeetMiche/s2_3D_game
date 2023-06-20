#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

typedef struct {
	int x1,y1;
	int x2,y2;
}walls; walls W[3000];

typedef struct {
	int ws, we;
	int z1, z2;
	int d;
	vector<int> surf;
	int surface;
}sectors; sectors S[100];

void import_walls(){
    FILE* wall_file = fopen("walls.txt", "r");
    if (wall_file != NULL){
        int i = 0;
        while (feof(wall_file) == 0){
            fscanf(wall_file, "%d,%d,%d,%d\n", &W[i].x1, &W[i].y1, &W[i].x2, &W[i].y2);
            i++;
        }
        fclose(wall_file);
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
        printf("\n\n Error while trying to open sector file!\n\n");
        exit(-1);
    }
}

// void import_sectors(){
//     ifstream sector_file("sectors.txt");
//     if (sector_file.is_open()){
//         char comma;
//         char endl;
//         sectors tmp;
//         while (sector_file >> tmp.ws >> comma >> tmp.we >> comma >> tmp.z1 >> comma >> tmp.z2 >> endl){
//             S.push_back(tmp);
//         }
//         sector_file.close();
//     }
// }

float distance(int x1, int x2, int y1, int y2){
	float d = sqrt(pow(x2 - x1, 2) + pow(y2-y1, 2));
	return d;
}

void sort_walls(int s){
	for (int i = S[s].ws; i < S[s].we; i++){
		for (int n = S[s].ws; n < S[s].we - i; n++){
			if (distance((W[n].x2 + W[n].x1)/2, P.x, (W[n].y2 + W[n].y1)/2, P.y) < distance((W[n+1].x2 + W[n+1].x1)/2, P.x, (W[n+1].y2 + W[n+1].y1)/2, P.y)) {
				walls swp = W[n + 1]; W[n + 1] = W[n]; W[n] = swp;
			}
		}
	}
}

#endif