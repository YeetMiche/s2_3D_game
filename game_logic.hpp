#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include "3dengine.hpp"
#include "2dengine.hpp"
#include "3dengine_classes.hpp"
#include "game_math.hpp"
#include "controlls.hpp"

extern int gameFrame;

int max_monster = 20;

class Monster{
    private:
    int spawn_distance = 3000;
    void generate_random_position(){
        switch(rand()%4){
        case 0:
            x = -spawn_distance + rand()%(spawn_distance/2);
            y = -spawn_distance + rand()%(spawn_distance/2);
            break;
        
        case 1:
            x = spawn_distance - rand()%(spawn_distance/2);
            y = -spawn_distance + rand()%(spawn_distance/2);
            break;

        case 2:
            x = -spawn_distance + rand()%(spawn_distance/2);
            y = spawn_distance - rand()%(spawn_distance/2);
            break;

        case 3:
            x = spawn_distance - rand()%(spawn_distance/2);
            y = spawn_distance - rand()%(spawn_distance/2);
            break;
        }
    }
    
    int moveToX, moveToY;
    
    public:
    bool isDead = false;
    int health = 100;
    int x,y;
    int texID = 5;
    int entID;

    Monster(bool isStrong = false){
        if (isStrong == true){
            health = 200;
            texID = 6;
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
                    if (factories <= 3 && P.coins >= 100){
                        factories += 1;
                        P.coins -= 100;
                    }
                    break;
                case 1:
                    if (coin_presses <= 3 && P.coins >= 1000){
                        coin_presses += 1;
                        P.coins -= 1000;
                    }
                    break;
                case 2:
                    if (nuclear_PPs <= 3 && P.coins >= 10000){
                        nuclear_PPs += 1;
                        P.coins -= 10000;
                    }
                    break;
            }
        }
    }

    void draw_items(){
        int text_height = get_text_sizey(2) * 3;
        draw_text(border*1.5, border*1.5, "BUY FACTORY (" + to_string(factories) + ")", 255,255,255,text_scale);
        draw_text(border*1.5, border*1.5 + text_height, "BUY PRESS  (" + to_string(coin_presses) + ")", 255,255,255,text_scale);
        draw_text(border*1.5, border*1.5 + 2 * text_height, "BUY NUCLEAR POWER PLANT  (" + to_string(nuclear_PPs) + ")", 255,255,255,text_scale);
        
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
    bool isOpen = false;

    Menu(){
        ;
    }

    void draw(){
        draw_BG();
        draw_items();
    }

    void update(){
        if (isOpen){draw();}
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
            cout << "Opening Menu!" << endl;
            menu.isOpen = true;
        }

        if (distance(P.x,x,P.y,y) > 300){menu.isOpen = false;}
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
        P.coins = 0;
    }

    void update(){
        display_coins();
        spawn_monster();

        for (int m = 0; m < monster_list.size(); m++){
            monster_list[m].update();
        }
        
        shopkeep.update();
        menu.update();
    }

    void display_coins(){
        draw_text(2,7,"Coins: " + to_string(P.coins), 200,250,30,2);
    }

    void spawn_monster(){
        
        if (monster_list.size() < max_monster && rand()%100 == 1){
            monster_list.push_back(Monster());
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