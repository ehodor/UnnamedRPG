#include <iostream>
#include <map>
#include <string>
#include "Char.h"
#include <ctime>
#include <cstdlib>
using namespace std;


void village(Char &player, location location) {
    cout << "Welcome to the village!" << endl;
    cout << "Here, you can visit the armorer (a), the general store (s), the weaponsmith (w)." << endl;
    cout << "If you're feeling brave, you can visit the lagoon (f) and continue your fighting journey, or press 'l' to"
            "see and go back to previous locations." << endl;

    cout << "Input: ";
    string input;
    cin >> input;
    if (input == "a") {

    }
}

void equipItem(Char &player, const string& item) {
    auto it = player.Inventory.find(item);
    if (it == player.Inventory.end()) {
        cout << "Item not found in inventory!" << endl;
    }
    else {
        auto it = player.inv_to_weapon.find(item);
        auto it1 = player.inv_to_helmet.find(item);
        if (it == player.inv_to_weapon.end() && it1 == player.inv_to_helmet.end()) {
            cout << "Item not equippable!" << endl;
        }
        else {
            if (it != player.inv_to_weapon.end()) {
                weapon weapon_to_equip = player.inv_to_weapon[item];
                if (player.weapon != "Bare hands") {
                    player.Inventory[player.weapon] += 1;
                }
                player.weapon = weapon_to_equip.name;
                player.weapon_strength = weapon_to_equip.strength;
                player.strength += weapon_to_equip.strength - player.strength + 1;
                player.Inventory[item] -= 1;
                if (player.Inventory[item] == 0) {
                    player.Inventory.erase(item);
                }
            }
            else if (it1 != player.inv_to_helmet.end()){
                helmet armor_to_equip = player.inv_to_helmet[item];
                player.helmet = armor_to_equip.defense;
                player.helmet_defense = armor_to_equip.defense;
                player.defense += player.helmet_defense - player.defense + 1;
                player.Inventory[item] -= 1;
                if (player.Inventory[item] == 0) {
                    player.Inventory.erase(item);
                }
            }
        }
    }
}

Enemy pickEnemy(location location) {
    srand(time(nullptr));
    float random = 1+ (rand() % 100);
    string temp;
    for ( const auto &[key, value]: location.enemies ) {
        if (random <= value) {
            temp = key;
        }
    }
    Enemy enemy_picked = location.str_to_en[temp];
    return enemy_picked;
}
void levelUp (int exp, Char &player) {
    if ((player.exp + exp) >= player.exp_to_level) {
        player.level++;
        player.exp =(player.exp +exp) - player.exp_to_level;
        player.exp_to_level += 5;
        player.max_health += 5;
        if (player.level % 2 == 0) {
            player.defense++;
        }
        cout << "You have leveled up! You are now level " << player.level << "!" << endl;
    }
    else {
        player.exp += exp;
    }
}


bool fight(Char &player, Enemy enemy_picked, int health) {
    player.health = health;
    bool defend = false;
    bool won = false;
    cout << "Enemy: " << enemy_picked.name << endl;
    while (true) {
        cout << "\n";
        cout << "Your health: " << player.health << endl;
        if (player.health <= 0) {
            cout << "You died." <<endl;
            player.health = player.max_health;
            break;
        }
        else if (enemy_picked.health <= 0) {
            cout << "You won! " << enemy_picked.exp << " experience and " << enemy_picked.coins << " coins gained!" <<endl;
            srand(time(nullptr));
            for ( const auto &[key, value]: enemy_picked.drops ) {
                float random = 1+ (rand() % 100);
                if (random <= value) {
                    player.Inventory[key] += 1;
                    cout << enemy_picked.name << " has dropped a " << key << "!" << endl;
                }
            }
            levelUp(enemy_picked.exp, player);
            //player.health = player.max_health;
            player.coins += enemy_picked.coins;
            won = true;
            break;
        }
        else {
            string action;
            cout << "Fight (f)      Defend (d)      Magic (m)      Potions (p)" << endl;
            cin >> action;
            if (action == "fight" || action == "f") {
                enemy_picked.health -= player.strength;
                cout << "You have dealt " << player.strength << " damage to " << enemy_picked.name << endl;
            }
            else if (action == "defend" || action == "d") {
                defend = true;
            }
            else if (action == "magic" || action == "m") {
                cout << "Spells: " << endl;
                for (const auto & spell : player.spells) {
                    cout << "   " << spell;
                }
                cout << "\n";
                while (true) {
                    cin >> action;
                    cout << "Input: ";
                    if (count(player.spells.begin(), player.spells.end(), action)) {
                        if (action == "identify") {
                            cout << enemy_picked.name << "  Strength: " << enemy_picked.strength << "   Defense: " << enemy_picked.defense
                                  << "    Health: " <<enemy_picked.health << "    Experience: " <<enemy_picked.exp << "   Money:" <<
                                  enemy_picked.coins << endl;
                            break;
                        }
                    }
                    else {
                        cout << "Not a valid spell!" << endl;
                    }
                }
            }
            else if (action == "potions" || action == "p") {
                cout << "What potion do you want to use?" << endl;
                string potion_chosen;
                for ( const auto &[key, value]: player.inv_to_potion ) {
                    cout << key << ": " << player.Inventory[key] << "    ";
                }
                cout << "Input: ";
                cin >> potion_chosen;
                cout << "\n";
                auto it = player.inv_to_potion.find(potion_chosen);
                if (it == player.inv_to_potion.end()) {
                    cout << "Not usable!" <<endl;
                }
                else {
                    health_potion potion = player.inv_to_potion[potion_chosen];
                    if (player.health + potion.health_restore > player.max_health) {
                        player.health = player.max_health;
                    }
                    else {
                        player.health += potion.health_restore;
                    }
                    player.Inventory[potion_chosen] -= 1;
                }
            }
            else {
                cout << "Invalid input!" << endl;
                continue;
            }
            if (defend) {
                if ((enemy_picked.strength / 2) - player.defense < 0) {
                    cout << enemy_picked.name << " has dealt 0 damage to you." << endl;
                }
                else {
                    player.health -= (enemy_picked.strength / 2) - player.defense;
                    defend = false;
                    cout << enemy_picked.name << " has dealt " << (enemy_picked.strength / 2) - player.defense <<
                    " damage to you." << endl;
                }
            }
            else {
                if (enemy_picked.strength - player.defense < 0) {
                    cout << enemy_picked.name << " has dealt 0 damage to you." << endl;
                }
                else {
                    player.health -= enemy_picked.strength - player.defense;
                    cout << enemy_picked.name << " has dealt " << enemy_picked.strength - player.defense <<
                    " damage to you." << endl;
                }
            }
        }
    }
    return won;
}

void dungeon(location location, Char &player) {
    bool temp;
    Enemy boss;
    bool won;
    class location dungeon_loc = location.dungeon_of_loc["dungeon"];
    for (int i=0;i<9;i++) {
        cout << "\nEnemy " << i+1 << " of 10" <<endl;
        Enemy enemy_picked = pickEnemy(dungeon_loc);
        won = fight(player, enemy_picked, player.health);
        if (!won) {
            temp = false;
            cout << "Your spirit is being teleported away from the dungeon." << endl;
            break;
        }
        temp = true;
    }
    if (temp) {
        for ( const auto &[key, value]: dungeon_loc.enemies ) {
            if (value == 0) {
                boss = dungeon_loc.str_to_en[key];
            }
        }
        won = fight(player, boss, player.health);
        if (!won) {
            cout << "You died. Your spirit is being teleported away from the dungeon." << endl;
        }
    }
}

int rpg(Char &player, const location& location) {
    cout << "Type 'help' or 'h' for help at any time, 'exit' or 'e' to quit the game, 'journey' or 'j' to "
            "embark on your journey, 'dungeon' to enter the dungeon to progress, or 'info' or 'c' "
            "for information on your character!"<< endl;
    cout << "\n";
    cout << "Your journey begins after escaping a prison you were held in after being framed. You're weak, "
            "weaponless, and only wearing basic torn rags...." << endl;
    cout << "You're lost in the forest, but stumble across a kind wizard...." << endl;
    cout << "'Hello, lost one. You seem like you're in trouble, here's a map to the nearest village and "
            "a special (reusable!) spell scroll. You figure out how to use it." << endl;
    cout << "There's a nice little shop in the village that can sell you everything you need, better hurry...'" << endl;



    while (true) {
        string input;
        cout << "Input: " ;
        cin >> input;
        if (input == "exit" || input == "e") {
            cout << "Goodbye, soldier!" << endl;
            break;
        }
        else if (input == "help" || input == "h") {
            cout << "Commands to use: help (h), character info (c), journey (f), dungeon, inventory (i), locations (l) "
                    "or exit (e). " << endl;
        }
        else if (input == "character" || input == "c") {
            cout << "Name: " << player.name << "    Level: " <<player.level  << "   Experience: " <<
            player.exp << "/" << player.exp_to_level << "   Coins: " << player.coins << endl;
            cout << "Strength: " << player.strength << "    Weapon: " << player.weapon << " (+" << player.weapon_strength << ")   Health: " <<
            player.health << "  Defense: "  << player.defense << endl;
            cout << "Spells: ";
            for (const auto & spell : player.spells) {
                cout << "   " << spell;
            }
            cout << "\n";
        }
        else if (input == "inventory" || input == "i") {
            cout << "Inventory: " << endl;
            for ( const auto &[key, value]: player.Inventory ) {
                cout << key << " Amount: " << value << "    ";
            }
            cout << "Type in 'equip' to enter equip menu!" <<endl;
        }
        else if (input == "equip") {
            cout << "Type in name of item you want to equip: ";
            string item;
            cin >> item;
            equipItem(player, item);
        }
        else if (input == "journey" || input == "f") {
            player.health = player.max_health;
            Enemy enemy_picked = pickEnemy(location);
            fight(player, enemy_picked, player.health);
            player.health = player.max_health;
        }
        else if (input == "dungeon") {
            dungeon(location, player);
        }
    }
    return 0;
};


int main() {
    std::cout << "Welcome to the Unnamed RPG!" << std::endl;
    std::cout << "What is your name, soldier?" << std::endl;
    Char player;
    cin >> player.name;
    cout << "Welcome, " << player.name << "! You have a great adventure ahead of you! \n" << endl;
    player.level = 1;
    player.defense = 0;
    player.health = 10;
    player.max_health = 10;
    player.exp = 0;
    player.exp_to_level = 10;
    player.strength = 1;
    player.coins = 0;
    player.weapon = "Bare hands";
    player.weapon_strength = 0;
    //item = player.Inventory.find()
    Enemy Goblin;
    strcpy(Goblin.name, "Goblin");
    Goblin.strength = 1;
    //Goblin.level = 1;
    Goblin.health = 5;
    Goblin.defense = 0;
    Goblin.exp = 5;
    Goblin.coins = 3;
    Goblin.drops["Wooden_Sword"] = 3;
    Goblin.drops["Twig"] = 40;
    Goblin.drops["hp5"] = 20;
    Goblin.drops["hp10"] = 8;

    Enemy Bear;
    ::strcpy(Bear.name, "Bear");
    Bear.strength = 2;
    //Bear.level = 1;
    Bear.health = 10;
    Bear.defense = 1;
    Bear.exp = 5;
    Bear.coins = 10;
    Bear.drops["Wooden_Sword"] = 25;
    Bear.drops["hp5"] = 40;
    Bear.drops["Stone_Sword"] = 5;
    Bear.drops["Bear_Claw"] = 70;
    Goblin.drops["hp10"] = 60;

    Enemy Troll;
    strcpy(Troll.name, "Troll");
    Troll.strength = 7;
    Troll.health = 25;
    Troll.defense = 4;
    Troll.exp = 20;
    Troll.drops["Troll_Cleaver"] = 50;
    Goblin.drops["hp10"] = 30;

    Enemy Troll_King;
    strcpy(Troll_King.name, "Troll_King");
    Troll_King.strength = 12;
    Troll_King.health = 70;
    Troll_King.defense = 6;
    Troll_King.exp = 200;
    Troll_King.drops["Troll_Cleaver"] = 75;
    Troll_King.drops["Gilded_Cleaver"] = 10;
    Troll_King.drops["hp10"] = 100;

    player.spells.emplace_back("identify");

    location deep_forest;
    strcpy(deep_forest.name, "deep_forest");
    deep_forest.enemies["Bear"] = 25;
    deep_forest.enemies["Troll"] = 100;
    deep_forest.enemies["Troll_King"] = 0;
    deep_forest.str_to_en["Bear"] = Bear;
    deep_forest.str_to_en["Troll"] = Troll;
    deep_forest.str_to_en["Troll_King"] = Troll_King;

    location light_forest;
    strcpy(light_forest.name, "light_forest");
    light_forest.enemies["Goblin"] = 90;
    light_forest.enemies["Bear"] = 100;
    light_forest.str_to_en["Goblin"] = Goblin;
    light_forest.str_to_en["Bear"] = Bear;
    light_forest.dungeon_of_loc["dungeon"] = deep_forest;

    location village_shop;
    strcpy(village_shop.name, "village_shop");

    item Twig{};
    strcpy(Twig.name, "Twig");
    Twig.price = 2;
    player.inv_to_item["Twig"] = Twig;

    item Bear_Claw{};
    strcpy(Bear_Claw.name, "Bear_Claw");
    Bear_Claw.price = 20;
    player.inv_to_item["Bear_Claw"] = Bear_Claw;


    weapon Wooden_Sword{};
    strcpy(Wooden_Sword.name, "Wooden_Sword");
    Wooden_Sword.price = 100;
    Wooden_Sword.strength = 2;
    player.inv_to_weapon["Wooden_Sword"] = Wooden_Sword;

    weapon Stone_Sword{};
    ::strcpy(Stone_Sword.name, "Stone_Sword");
    Stone_Sword.price = 500;
    Stone_Sword.strength = 5;
    player.inv_to_weapon["Stone_Sword"] = Stone_Sword;

    weapon Troll_Cleaver{};
    strcpy(Troll_Cleaver.name, "Troll_Cleaver");
    Troll_Cleaver.price = 1500;
    Troll_Cleaver.strength = 10;
    player.inv_to_weapon["Troll_Cleaver"] = Troll_Cleaver;

    weapon Gilded_Cleaver{};
    strcpy(Gilded_Cleaver.name, "Gilded_Cleaver");
    Gilded_Cleaver.price = 10000;
    Gilded_Cleaver.strength = 15;
    player.inv_to_weapon["Gilded_Cleaver"] = Gilded_Cleaver;

    health_potion hp5{};
    ::strcpy(hp5.name, "hp5");
    hp5.price = 20;
    hp5.health_restore = 5;
    player.inv_to_potion["hp5"] = hp5;

    health_potion hp10{};
    ::strcpy(hp10.name, "hp10");
    hp10.price = 50;
    hp10.health_restore = 10;
    player.inv_to_potion["hp10"] = hp10;

    helmet Leather_Helmet;
    strcpy(Leather_Helmet.name, "Leather_Helmet");


    shop armorer;
    strcpy(armorer.name, "armorer");
    armorer.items["Leather_Helmet"] = 5000;
    armorer.items["Leather_Chestplate"] = 10000;
    armorer.items["Leather_Pants"] = 7500;
    armorer.items["Leather_Boots"] = 5000;



    rpg(player, light_forest);
    return 0;
}
