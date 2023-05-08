//
// Created by Emma Hodor on 4/30/23.
//
#include <iostream>
#include <map>
#include <string>
#include "Char.h"
using namespace std;

void enemies( ) {
    extern Enemy Goblin;
    strcpy(Goblin.name, "Goblin");
    Goblin.strength = 1;
    Goblin.level = 1;
    Goblin.health = 5;
    Goblin.defense = 0;

    extern Enemy Bear;
    ::strcpy(Bear.name, "Bear");
    Bear.strength = 2;
    Bear.level = 1;
    Bear.health = 10;
    Bear.defense = 1;
}