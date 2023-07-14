#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include "2dengine.hpp"
#include "3dengine_classes.hpp"

class Monster{
    private:
    int distance = 4000;
    void generate_random_position(){
        switch(rand()%4){
        case 0:
            x = -distance - rand()%1000;
            y = -distance - rand()%1000;
            break;
        
        case 1:
            x = distance + rand()%1000;
            y = -distance - rand()%1000;
            break;

        case 2:
            x = -distance - rand()%1000;
            y = distance + rand()%1000;
            break;

        case 3:
            x = distance + rand()%1000;
            y = distance + rand()%1000;
            break;
        }
    }
    
    public:
    bool isDead = false;
    int health = 100;
    int x,y;
    int texID = 5;
    int objID;

    Monster(bool isStrong = false){
        if (isStrong == true){
            health = 200;
            texID = 6;
        }

        generate_random_position();

        Obj.push_back(Object(x,y,50,texID,2));
        objID = Obj.size();
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
        
        if (monster_list.size() <= 20 && rand()%100 == 1){
            monster_list.push_back(Monster());
        }
        for (int i = 0; i < monster_list.size(); i++){
            if (monster_list[i].isDead) {
                Obj.erase(Obj.begin() + monster_list[i].objID);
                monster_list.erase(monster_list.begin() + i);
            }
        }
    }
};

Game game;

#endif /*GAME_LOGIC_H_*/