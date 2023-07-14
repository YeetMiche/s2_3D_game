#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include "2dengine.hpp"
#include "3dengine_classes.hpp"
#include "game_math.hpp"

int max_monster = 1;

class Monster{
    private:
    int spawn_distance = 4000;
    void generate_random_position(){
        switch(rand()%4){
        case 0:
            x = -spawn_distance + rand()%2000;
            y = -spawn_distance + rand()%2000;
            break;
        
        case 1:
            x = spawn_distance - rand()%2000;
            y = -spawn_distance + rand()%2000;
            break;

        case 2:
            x = -spawn_distance + rand()%2000;
            y = spawn_distance - rand()%2000;
            break;

        case 3:
            x = spawn_distance - rand()%2000;
            y = spawn_distance - rand()%2000;
            break;
        }
    }
    
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
        entID = rand()%100000;
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

    void update(){
        check_shot();
    }
};

vector<Monster> monster_list;

class Game{
    public:

    int coins;

    Game(){
        coins = 0;
    }

    void update(){
        display_coins();
        spawn_monster();

        for (int m = 0; m < monster_list.size(); m++){
            monster_list[m].update();
        }
    }

    void display_coins(){
        draw_text(10,10,"Coins: " + to_string(coins), 200,250,30);
    }

    void spawn_monster(){
        
        if (monster_list.size() < max_monster && rand()%100 == 1){
            monster_list.push_back(Monster());
        }
        for (int i = 0; i < monster_list.size(); i++){
            if (monster_list[i].isDead) {
                coins += 10;
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