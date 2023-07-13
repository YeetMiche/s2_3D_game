#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include "2dengine.hpp"
#include "3dengine_classes.hpp"

class Monster{
    private:
    void generate_random_position(){
        switch(rand()%4){
        case 0:
            x = -4000 + rand()%500 - rand()%500;
            y = -4000 + rand()%500 - rand()%500;
            break;
        
        case 1:
            x = 4000 + rand()%500 - rand()%500;
            y = -4000 + rand()%500 - rand()%500;
            break;

        case 2:
            x = -4000 + rand()%500 - rand()%500;
            y = 4000 + rand()%500 - rand()%500;
            break;

        case 3:
            x = 4000 + rand()%500 - rand()%500;
            y = 4000 + rand()%500 - rand()%500;
            break;
        }
    }
    
    public:
    bool isDead = false;
    int health = 100;
    int x,y;
    int texID = 5;

    Monster(bool isStrong = false){
        if (isStrong == true){
            health = 200;
            texID = 6;
        }

        generate_random_position();
    }

    void draw(){
        draw_texture_3D(texture_list[texID], x,y,50,2);
    }
};

vector<Monster> monster_list;

class Game{
    public:

    int coins;

    Game(){
        coins = 0;
    }

    void display_coins(){
        draw_text(10,10,"Coins: " + to_string(coins), 200,250,30);
    }

    void spawn_monster(){
        monster_list.push_back(Monster());

        for (int i = 0; i < monster_list.size(); i++){
            if (monster_list[i].isDead) {
                monster_list.erase(monster_list.begin() + i);
            }
        }
    }

    void draw_monster(){
        for (int i = 0; i < monster_list.size(); i++){
            monster_list[i].draw();
        }
    }
};

Game game;

#endif /*GAME_LOGIC_H_*/