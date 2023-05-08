//
// Created by Emma Hodor on 4/30/23.
//

#ifndef UNTITLED_CHAR_H
#define UNTITLED_CHAR_H
#include <iostream>
#include <map>
#include <string>
using namespace std;

class weapon {
public:
    char name[20];
    int strength;
    int price;
};

class item {
public:
    char name[20];
    int price;
};
class helmet {
public:
    char name[20];
    int price;
    int defense;
};


class health_potion {
public:
    char name[20];
    int price;
    int health_restore;
};
class Char {
public:
    char name[20];
    std::map<std::string , int> Inventory;
    map<string, weapon> inv_to_weapon;
    map<string, item> inv_to_item;
    map<string, health_potion> inv_to_potion;
    map<string, helmet> inv_to_helmet;
    int level;
    int exp;
    int exp_to_level;
    int strength;
    int defense;
    int health;
    int max_health;
    int coins;
    //void getInfo() {};
    //void getInv() {};
    string weapon;
    string helmet;
    string chestplate;
    string pants;
    string shoes;
    int weapon_strength;
    int helmet_defense;
    vector<string> spells;
};

class Enemy {
public:
    char name[20];
    int level;
    int strength;
    int defense;
    int health;
    int exp;
    int coins;
    map<string, float> drops;

};



class location {
public:
    char name[20];
    map<string, float> enemies;
    map<string, Enemy> str_to_en;
    map<string, location> dungeon_of_loc;
};

class shop {
public:
    char name[20];
    map<string, int> items;
};



#endif //UNTITLED_CHAR_H
