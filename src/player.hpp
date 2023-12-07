/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka, Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Description: The player class is derrived from the module class, however is the only module
                that is not a child of another module. The player class is the main class that
                controls the player and all of its modules. The player class is also the entry
                point for the player to be rendered to the screen. The player class also
                contains the playerStatus struct which is used to store the current status
                of the player and is used to determine what the player is doing for other
                modules to determine their functionality. The Player Object Can Shoot.
*******************************************************************************************/
#pragma once
#include "../imgui/imgui.h"
#include <iostream>
#include <math.h>

//Forward declarations
class bullet;
class enemy;
class module;
class gun;
class gameObject;

//Player status struct
struct playerStatus {
    bool isMoving;
    bool isFiring;
    bool isInventory;
};

#include "module.hpp"
#include "gameobj.hpp"
class player : public module {
public:
    //Constructors and destructor
    player();
    player(ImVec2 pos, gameObject* g);
    ~player() {};

    //Main object entry update function
    void update(std::vector<enemy*>& gameEnemies, gameObject* gObj);

    //Settors and Gettors
    void addKill();                         //Adds a kill to the player's score
    void addGun(gun* g);                    //Adds pointer to a gun module to the player
    void removeGun(gun* g);                 //Removes a gun module from the player
    void addBalance(int m);                 //Updates the player's balance
    void addThruster(float da, float ds);   //Adds thruster to the player

    int getKills();                         //Returns the player's kills
    float getHealth();                      //Returns the player module's health
    int getBandCount();                     //Returns the player's balance
    bool getIsMoving();                     //Returns if the player is moving
    ImVec2 getPosition();                   //Returns the player's position
    float getTimeAlive();                   //Returns the player's time alive
    bool getIsInventory();                  //Returns if the player is in the inventory to pause game updates
    module* getFirstModule();               //Returns the child module for root of ship tree
    ImDrawList* getPlayerDrawList();        //Returns the main draw list stored in the player
    ImVec2 getEnemy_PlayerHeading();        //Returns the psuedo position of the player for enemy calculations

private:

    void showNodes();                       //Draws the player module's node
    void fireBullet();                      //Fires a bullet from the player and all gun modules
    void drawPlayer();                      //Draws the player to the screen
    void handleEvents();                    //Handles IO inputs
    void CalculatePosition(ImVec2 accel);   //Calculates the player's position per frame

//Private variables
    int kills;
    ImVec2 acc;
    int C_a = 5;
    float angle;
    uint balance;
    float friction;
    float maxSpeed;
    float timeAlive;
    ImVec2 velocity;
    ImDrawList* drawList;
    playerStatus curStatus;
    float headingDifferential;
    std::vector<gun*> pewpews;
    bool headingDifferentialSet;
    std::vector<bullet*> bullets;
};


/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka, Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Description: The Enemy class is derrived from the player as it needs to be able to control
                itself in a similar fashion as the player, and not just a static module. The
                Enemies are owned by the game object and are updated by the game object. They
                Only shoot when the player is in range and are constantly moving towards the
                player "position" which is a psuedo position that is calculated by the game
                to appear as if it is being controlled.

*******************************************************************************************/
class enemy : public player {
public:
    enemy() {};
    enemy(ImVec2 pos);
    ~enemy() {
        for (int i = 0; i < EnemyBullets.size(); i++) {
            delete EnemyBullets[ i ];
        }
    };

    //Main Bot update function
    void update(ImVec2& p1Position, player& p1);

    void drawEnemy(ImVec2& p1Position, ImDrawList* dl);     //Draws the enemy to the screen
    void fireBullet();                                      //Fires a bullet from the enemy
    ImVec2 getPosition();                                   //Returns the enemy's position
    float getTimeUntilNextShot();                           //Returns the time until the enemy can shoot again
    void bulletCollisions(module* m, int bullID);           //detects if a bullet has collided with a module

private:

    inline float distFromPlayer(ImVec2& playerPos);                //Returns the distance from the player for collision detection and deletion
    void calcEnemyPosition(ImVec2& p1Position);             //Updates the enemy's position per frame

    float radius;
    float maxSpeed = 10;
    float friction = 0.2;
    float EnemyAngle;
    float EnemyAngleDt;
    ImVec2 position;
    ImVec2 velocity;
    float acc = 5;
    float timeUntilNextShot;
    std::vector<bullet*> EnemyBullets;

};
