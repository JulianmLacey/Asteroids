/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Description: This class is the gameObject class which is the main container for the game
                The main loop only enstantiates one gameObject and then calls the update
                function on it when play is active. Additionally is the Background star struct
                which is used to store the star data for the background and only used on this
                file.
*******************************************************************************************/
#pragma once
#include "../imgui/imgui.h"
#include <sstream>
#include "module.hpp"
#include "player.hpp"
#include "testcases.hpp"

class gun;
class player;

//Background star struct to store the data for the stars and generate them
struct backgroundStar {
    backgroundStar(ImVec2& pos, int n) {
        position.x = pos.x;
        position.y = pos.y;
        size = rand() % n + 1;
        alpha = ((float)(rand() % 100 + 1)) / 200.0f;
    }
    ImVec2 position;
    float size;
    float alpha;
};
class gameObject {
public:
    //Constructors and destructor
    gameObject() {};
    gameObject(ImVec2 ScreenSize);
    ~gameObject() {
        enemies.clear();
        foregroundStars.clear();
        backgroundStars.clear();
    };

    //main game loop
    void updateGame();
    void initGame();

    //Getters and setters to externally update the game
    player* getPlayer();         //Returns the games player object
    void addEnemy();             //Adds an enemy to the game
    void killEnemy(enemy* e);    //Removes enemy from the game
    void addGun(gun* g);         //Adds a Gun Module to the player
    void increaseLevel();        //Increases the level of the game
    void resetGame();            //Resets the game
    void runTests();

private:

    void drawBackground();       //Renders the background stars

    player* p1;
    std::vector<enemy*> enemies;
    std::vector<backgroundStar*> foregroundStars;
    std::vector<backgroundStar*> backgroundStars;
    ImVec2 screenDIM;
    float timeUntilNextEnemy;
    int level;
    int maxEnemies;
    Tests* test;
};



