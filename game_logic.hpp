#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "3dengine.hpp"
#include "2dengine.hpp"
#include "3dengine_classes.hpp"
#include "world_classes.hpp"
#include "game_math.hpp"
#include "controlls.hpp"

extern int gameFrame;
extern const int FPS_LIMIT;
extern const int player_start_coins;

int max_monster = 20;

int import_building(string wall_file_name, string sector_file_name, int xo = 0, int yo = 0){
    vector<Sector> sectors;
    vector<Wall> walls;

    int secID;

    ifstream wall_file(wall_file_name);

    if (wall_file.is_open()){
        char comma;
        int x1,y1,x2,y2;
        while (wall_file >> x1 >> comma >> y1 >> comma >> x2 >> comma >> y2){
            walls.push_back(Wall(x1 + xo,y1 + yo,x2 + xo,y2 + yo));
        }
        wall_file.close();
    }

    ifstream sector_file(sector_file_name);

    if (sector_file.is_open()){
        char comma;
        int ws,we,z1,z2;
        int texID;
        int isFloor;
        while (sector_file >> ws >> comma >> we >> comma >> z1 >> comma >> z2 >> comma >> texID >> comma >> isFloor){
            sectors.push_back(Sector(ws,we,z1,z2,isFloor,texID));
            
        }

        sector_file.close();
    }

    for (int s = 0; s < sectors.size(); s++){
        int wsa = W.size();
        for (int w = sectors[s].ws; w <= sectors[s].we; w++){
            W.push_back(Wall(walls[w].x1, walls[w].y1, walls[w].x2, walls[w].y2));
        }
        int wea = W.size() - 1;
        secID = rand();
        S.push_back(Sector(wsa,wea, sectors[s].z1, sectors[s].z2, sectors[s].isFloor, sectors[s].textureID,1,1,secID));
    }

    return secID;
}

class Monster{
    private:
    int spawn_distance = 3000;
    void generate_random_position(){
        bool collides = true;
        while(collides){
            int force_quit = 0;
            switch(rand()%2){
            case 0:
                x = -spawn_distance + rand()%(spawn_distance/2);
                y = -spawn_distance + rand()%(spawn_distance/2);
                break;

            case 1:
                x = -spawn_distance + rand()%(spawn_distance/2);
                y = spawn_distance - rand()%(spawn_distance/2);
                break;
            }

            collides = false;
            for (int w = 0; w < W.size(); w++){
                if (isCollision(x,y,100,W[w].x1,W[w].y1,W[w].x2,W[w].y2)) {collides = true;}
            }
            force_quit += 1; 
            if (force_quit > 500) {break;}
        }
    }
    
    int moveToX, moveToY;
    
    public:
    bool isDead = false;
    int health = 100;
    int x,y;
    int texID = 5;
    int entID;

    bool isStrong;

    Monster(bool _isStrong = false){
        if (_isStrong == true){
            health = 200;
            texID = 6;
            isStrong = true;
        }



        generate_random_position();
        findValidPosition();
        entID = rand();
        Obj.push_back(Object(x,y,50,texID,2, entID));

    }

    void check_shot(){
        if (P.shooting){
            int da = abs(calculate_angle(-P.x,P.y,-x,y) + 90 - P.a) - 180;
            int range;
            range = 2000 / distance(P.x, x, P.y, y);

            if (da >= -range && da <= range){
                health -= 10;

                if (isStrong){
                    P.coins += 10;
                }
            }

            if (health <= 0){
                isDead = true;
            }
        }
    }

    void findValidPosition(){
        bool collides = true;
        int force_quit = 0;
        while (collides){
            moveToX = x + randPosNeg(700);
            moveToY = y + randPosNeg(700);
            collides = false;
            for (int w = 0; w < W.size(); w++){
                if (isCollision(x,y,100,W[w].x1,W[w].y1,W[w].x2,W[w].y2)) {collides = true;}
            }
            force_quit += 1; 
            if (force_quit > 500) {break;}
        }
    }

    void moveRandom(){
        if (inRange(x, moveToX-10, moveToX+10) && inRange(y, moveToY-10, moveToY+10)){
            if (rand()%500 == 0){
                findValidPosition();
            }
        } else {
            float da = calculate_angle(x,y,moveToX,moveToY) + 90;
            float dx = sin(da/180*M_PI) * 5.0;
            float dy = cos(da/180*M_PI) * 5.0;

            x += dx, y -= dy;
        }

        

        int find = entID;
        for (int f = 0; f < Obj.size(); f++){
            if (Obj[f].entID == find){Obj[f].x = x; Obj[f].y = y;}
        }
    }

    void update(){
        check_shot();
        moveRandom();
    }
};

vector<Monster> monster_list;

class SmokeClouds{
    private:
    int x,y,z;
    float scale;

    public:
    int entID;
    bool is_dead = false;

    SmokeClouds(int _x, int _y, int _z){
        x = _x;
        y = _y;
        z = _z;
        
        entID = rand();
        scale = 1 + randPosNeg(10) / 50.0;
        Obj.push_back(Object(x,y,z,12,1, entID));
    }

    void update(){
        z -= 5 + randPosNeg(1);
        x += rand()%2;
        y += rand()%3;

        scale -= (rand()%10) / 500.0;

        if (scale <= 0){is_dead = true;}

        int find = entID;
        for (int f = 0; f < Obj.size(); f++){
            if (Obj[f].entID == find){
                Obj[f].x = x;
                Obj[f].y = y;
                Obj[f].z = z;
                Obj[f].scale = scale; 
            }
        }
    }
};

vector<SmokeClouds> smoke;

class Factory{
    private:
    float internal_coins = 0;
    string wall_file = "./buildings/factory_w.txt";
    string sector_file = "./buildings/factory_s.txt";

    int xo,yo;

    public:
    Factory(int _xo = 0, int _yo = 0){
        import_building(wall_file, sector_file, _xo, _yo);
        xo = _xo;
        yo = _yo;
    }

    void update(){
        internal_coins += 1.0 / FPS_LIMIT;
        
        if (internal_coins >= 1){
            internal_coins = 0;
            P.coins += 1;
        } 

        if (rand()%80 == 0){
            smoke.push_back(SmokeClouds(600 + xo,-1046 + yo,-400));
        }

    }
};

vector<Factory> factory_list;

class CoinPress{
    private: 
    float internal_coins = 0;
    string wall_file = "./buildings/press_w.txt";
    string sector_file = "./buildings/press_s.txt";

    int movableSecID;

    int pressZ = 0;

    int xo,yo;

    int timeOffset;

    public:
    CoinPress(int _xo = 0, int _yo = 0){
        movableSecID = import_building(wall_file, sector_file, _xo, _yo);
        xo = _xo;
        yo = _yo;

        timeOffset = rand();
    }

    void update(){
        internal_coins += 10.0 / FPS_LIMIT;

        pressZ = sin(gameFrame/40.0 + timeOffset) * 40 - 80;
        

        int find = movableSecID;
        for (int f = 0; f < S.size(); f++){
            if (S[f].secID == find){
                S[f].z1 = pressZ;
                S[f].z2 = pressZ + 80;
            }
        }


        if (internal_coins >= 1){
            internal_coins = 0;
            P.coins += 1;
        }
    }
};

vector<CoinPress> press_list;

class NuclearPowerPlant{
    private:
    float internal_coins = 0;
    string wall_file = "./buildings/nuclearPP_w.txt";
    string sector_file = "./buildings/nuclearPP_s.txt";

    int xo,yo;

    public:
    NuclearPowerPlant(int _xo = 0, int _yo = 0){
        import_building(wall_file, sector_file, _xo, _yo);
        xo = _xo;
        yo = _yo;
    }

    void update(){
        internal_coins += 100.0 / FPS_LIMIT;

        if (internal_coins >= 1){
            internal_coins = 0;
            P.coins += 1;
        }
    }
};

vector<NuclearPowerPlant> nuclearPP_list;

class Menu{
    private:
    int border = 100 / render_scale; 
    int factories = 0;
    int coin_presses = 0;
    int nuclear_PPs = 0;

    int cursor_pos = 0;
    float text_scale = 2.5;

    void draw_BG(){
        glBegin(GL_QUADS);
            glColor3ub(60,40,40);
            glVertex2i(border-2,border-2);
            glVertex2i(window_x - border + 2,border - 2);

            glColor3ub(20,20,40);
            glVertex2i(window_x - border + 2, window_y - border + 2);
            glVertex2i(border - 2, window_y - border + 2);
        glEnd();
        
        glBegin(GL_QUADS);
            glColor3ub(50,50,50);
            glVertex2i(border,border);
            glVertex2i(window_x - border,border);

            glColor3ub(30,30,35);
            glVertex2i(window_x - border, window_y - border);
            glVertex2i(border, window_y - border);
        glEnd();
    }

    void menu_control(){
        if (Keys.ad == 1){
            cursor_pos += 1;
        }

        if (Keys.au == 1){
            cursor_pos -= 1;
        }

        if (cursor_pos > 2){
            cursor_pos = 0;
        }
        if (cursor_pos < 0){
            cursor_pos = 2;
        }

        if (Keys.ar == 1){
            switch(cursor_pos){
                case 0:
                    if (factories < 10 && P.coins >= 100){
                        factories += 1;
                        P.coins -= 100;
                        factory_list.push_back(Factory(factories * 500));
                    }
                    break;
                case 1:
                    if (coin_presses < 10 && P.coins >= 1000){
                        coin_presses += 1;
                        P.coins -= 1000;
                        press_list.push_back(CoinPress(coin_presses * 500));
                    }
                    break;
                case 2:
                    if (nuclear_PPs < 3 && P.coins >= 10000){
                        nuclear_PPs += 1;
                        P.coins -= 10000;
                        nuclearPP_list.push_back(NuclearPowerPlant(nuclear_PPs * 2500));
                    }
                    break;
            }
        }
    }

    void draw_items(){
        int text_height = get_text_sizey(2) * 3;
        draw_text(border*1.5, border*1.5, "BUY FACTORY [1 coin per second] (" + to_string(factories) + ")", 255,255,255,text_scale);
        draw_text(border*1.5, border*1.5 + text_height, "BUY PRESS [10 coins per second] (" + to_string(coin_presses) + ")", 255,255,255,text_scale);
        draw_text(border*1.5, border*1.5 + 2 * text_height, "BUY NUCLEAR POWER PLANT [100 coins per second] (" + to_string(nuclear_PPs) + ")", 255,255,255,text_scale);
        
        switch(cursor_pos){
            case 0:
                draw_text(border*1.5, border*1.5, "BUY FACTORY", 255,255,0,text_scale);
                break;
            case 1:
                draw_text(border*1.5, border*1.5 + text_height, "BUY PRESS", 255,255,0,text_scale);
                break;
            case 2:
                draw_text(border*1.5, border*1.5 + 2 * text_height, "BUY NUCLEAR POWER PLANT", 255,255,0,text_scale);
                break;
        }

        draw_text(window_x - border*1.5 - get_text_sizex("100 COINS", text_scale), border*1.5, "100 COINS", 255,255,255,text_scale);
        draw_text(window_x - border*1.5 - get_text_sizex("1k COINS", text_scale), border*1.5 + text_height, "1k COINS", 255,255,255,text_scale);
        draw_text(window_x - border*1.5 - get_text_sizex("10k COINS", text_scale), border*1.5 + 2 * text_height, "10k COINS", 255,255,255,text_scale);

        if (P.coins < 100){
            draw_text(window_x - border*1.5 - get_text_sizex("100 COINS", text_scale), border*1.5, "100 COINS", 255,50,50,text_scale);
        }

        if (P.coins < 1000){
            draw_text(window_x - border*1.5 - get_text_sizex("1k COINS", text_scale), border*1.5 + text_height, "1k COINS", 255,50,50,text_scale);
        }

        if (P.coins < 10000){
            draw_text(window_x - border*1.5 - get_text_sizex("10k COINS", text_scale), border*1.5 + 2 * text_height, "10k COINS", 255,50,50,text_scale);
        }
    }

    public:
    bool is_open = false;

    Menu(){
        ;
    }

    void draw(){
        draw_BG();
        draw_items();
    }

    void update(){
        if (is_open){draw();}
        menu_control();
    }
};

Menu menu;

class ShopKeep{
    public:
    int x = 226;
    int y = 552;
    int z = 50;
    float scale = 1.2;
    int texID = 11;

    ShopKeep(){
        Obj.push_back(Object(x,y,z,texID,scale));
    }

    void draw_text(){
        if (distance(P.x,x,P.y,y) < 250){
            draw_text_3D("Shop Keep",x,y,z-50,{150,170,120});
        }
    }

    void open_menu(){
        if (distance(P.x,x,P.y,y) < 250 && Keys.interact == 1){
            menu.is_open = true;
        }

        if (distance(P.x,x,P.y,y) > 300 || Keys.close == 1){menu.is_open = false;}
    }

    void update(){
        draw_text();
        open_menu();
    }
};

ShopKeep shopkeep;

class Game{
    public:

    Game(){
        P.coins = player_start_coins;
    }

    void update(){
        display_coins();
        spawn_monster();

        for (int m = 0; m < monster_list.size(); m++){
            monster_list[m].update();
        }
        
        for (int f = 0; f < factory_list.size(); f++){
            factory_list[f].update();
        }


        for (int p = 0; p < press_list.size(); p++){
            press_list[p].update();
        }

        for (int i = 0; i < nuclearPP_list.size(); i++){
            nuclearPP_list[i].update();
        }
        
        for (int s = 0; s < smoke.size(); s++){
            smoke[s].update();

            if (smoke[s].is_dead){
                int find = smoke[s].entID;
                for (int f = 0; f < Obj.size(); f++){
                    if (Obj[f].entID == find){
                        Obj.erase(Obj.begin() + f);
                    }
                }
                smoke.erase(smoke.begin() + s);
            }
        }

        shopkeep.update();
        menu.update();
        
    }

    void display_coins(){
        draw_text(2,7,"Coins: " + to_string(P.coins), 200,250,30,2);
    }

    void spawn_monster(){
        
        if (monster_list.size() < max_monster && rand()%70 == 1){
            if (rand()%10 == 0){
                monster_list.push_back(Monster(true));
            } else {
                monster_list.push_back(Monster());
            }
            
        }
        for (int i = 0; i < monster_list.size(); i++){
            if (monster_list[i].isDead) {
                P.coins += 10;
                int find = monster_list[i].entID;
                for (int f = 0; f < Obj.size(); f++){
                    if (Obj[f].entID == find){
                        Obj.erase(Obj.begin() + f);
                    }
                }
                monster_list.erase(monster_list.begin() + i);
            }
        }
    }
};

Game game;



#endif /*GAME_LOGIC_H_*/