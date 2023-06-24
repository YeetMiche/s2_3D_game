#ifndef WORLD_CLASSES_
#define WORLD_CLASSES_

#include <vector>
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
    int textureID;
    float uscale, vscale;

    Sector(int _ws, int _we, int _z1, int _z2, int _textureID = 0, float _uscale = 1, float _vscale = 1){
        ws = _ws;
        we = _we;
        z1 = _z1;
        z2 = _z2;
        textureID = _textureID;
        uscale = _uscale;
        vscale = _vscale;
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


#endif