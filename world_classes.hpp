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
    int distance;

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
    int textureID;
    float uscale, vscale;
    bool isFloor = false;
    int secID;

    Sector(int _ws, int _we, int _z1, int _z2, int _isFloor, int _textureID = 0, float _uscale = 1, float _vscale = 1, int _secID = -1){
        ws = _ws;
        we = _we;
        z1 = _z1;
        z2 = _z2;
        if (_isFloor == 1) {isFloor = true;}
        textureID = _textureID;
        uscale = _uscale;
        vscale = _vscale;
        secID = _secID;
    }
};

vector<Sector> S;

class Object{
    public:
    int x,y,z;
    float scale;
    int textureID;
    bool isEnemy = false;
    int entID;
    bool beenDrawn = false;

    Object(int _x, int _y, int _z, int _textureID = 0, float _scale = 1, int _entID = -1){
        x = _x, y = _y, z = _z;
        textureID = _textureID;
        scale = _scale;
        entID = _entID;
    }
};

vector<Object> Obj;

#endif