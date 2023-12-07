#include "gameobj.hpp"
#include "time.h"
#include "testcases.hpp"

/*******************************************************************************************
*****************************        GAMEOBJECT CLASS          *****************************
*******************************************************************************************/

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::gameObject
* Description: gameObject constructor, initializes game variables and creates player instance
* Input Parameters: ImVec2 ScreenSize - screen dimensions in pixels
* Return Value: N/A
*******************************************************************************************/
gameObject::gameObject(ImVec2 ScreenSize) {
    //initialize game variables
    this->screenDIM = ScreenSize;
    this->timeUntilNextEnemy = 0;
    this->maxEnemies = 2;
    this->level = 1;

    //create player instance
    this->p1 = new player(ImVec2(ScreenSize.x / 2, ScreenSize.y / 2), this);

    //seed time
    srand(time(NULL));

    //generate background
    int b = 0;
    for (b = 0; b < 30; b++) {
        ImVec2 p = ImVec2(rand() % (int)screenDIM.x / 2, rand() % (int)screenDIM.y / 2);
        foregroundStars.push_back(new backgroundStar(p, 3));
    }
    for (b = 0; b < 50; b++) {
        ImVec2 p = ImVec2(rand() % (int)screenDIM.x, rand() % (int)screenDIM.y);
        backgroundStars.push_back(new backgroundStar(p, 2));
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::initGame
* Description: Initializes Game
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void gameObject::initGame() {
    //create player ship
    module* mod1 = new truss(ImVec2(0, 0), 0, (void*)this->p1, (void*)this);
    thruster* newT = new thruster(ImVec2(0, 0), 0, (void*)mod1, (void*)this);
    mod1->setNodeModule((void*)newT, 0);
    this->p1->setNodeModule((void*)mod1, 0);

    //add starting enemy
    this->addEnemy();
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::resetGame
* Description: Initializes Game
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void gameObject::resetGame() {
    //delete player and enemies
    delete p1;
    enemies.clear();

    //create new player and ship
    p1 = new player(ImVec2(screenDIM.x / 2, screenDIM.y / 2), this);
    module* mod1 = new truss(ImVec2(0, 0), 0, (void*)this->p1, (void*)this);
    thruster* newT = new thruster(ImVec2(0, 0), 0, (void*)mod1, (void*)this);

    //add starting enemy
    this->addEnemy();
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::getPlayer
* Description: Gettor for the player instance
* Input Parameters: N/A
* Return Value: player*
*******************************************************************************************/
player* gameObject::getPlayer() {
    return p1;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::increaseLevel
* Description: Updates the games level
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void gameObject::increaseLevel() {
    //increase level and increase max enemies every 5 levels
    level++;
    if (level % 5 && level != 0) {
        maxEnemies++;
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::drawBackground
* Description: Draws the background stars to the screen
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void gameObject::drawBackground() {
    //get current draw list
    ImDrawList* dl = ImGui::GetWindowDrawList();

    //loop through stars and draw them
    for (int i = 0; i < backgroundStars.size(); i++) {
        ImColor c = ImColor(1.0f, 1.0f, 1.0f, 1.0f * backgroundStars[ i ]->alpha);
        dl->AddCircleFilled(backgroundStars[ i ]->position, backgroundStars[ i ]->size, c);
    }
    for (int i = 0; i < foregroundStars.size(); i++) {
        ImColor c = ImColor(1.0f, 1.0f, 1.0f, 1.0f * foregroundStars[ i ]->alpha);
        dl->AddCircleFilled(foregroundStars[ i ]->position, foregroundStars[ i ]->size, c);
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::updateGame
* Description: main game update function, updates player and enemies
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void gameObject::updateGame() {
    this->drawBackground();                                 //Draw background stars
    p1->update(this->enemies, this);                        //update the player
    ImVec2 PlayerPosition = p1->getEnemy_PlayerHeading();   //get pseudo player position
    ImVec2 PlayerPositionReal = p1->getPosition();          //get actual player position

    //spawn new enemies and update all enemies
    if (!p1->getIsInventory()) {                                        //Check if player is in inventory to pause game updates
        timeUntilNextEnemy -= ImGui::GetIO().DeltaTime;                 //reduce time until next enemy by time since last frame
        if (timeUntilNextEnemy <= 0 && enemies.size() < maxEnemies) {   //if time until next enemy is less than 0 and there are less than max enemies allowed
            //add enemy add reset time proportional to level
            this->addEnemy();
            timeUntilNextEnemy = 10 / level;
        }
        //if there are enemies in the game, update them
        if (enemies.size() >= 1) {
            for (auto a = enemies.begin(); a != enemies.end(); a++) {
                (*a)->update(PlayerPosition, *(this->p1));
            }
        }
    }

}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::addEnemy
* Description: Settor for game enemies
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void gameObject::addEnemy() {
    //random position in window
    int posX = rand() % (int)screenDIM.x;
    int posY = rand() % (int)screenDIM.y;

    //add enemy to gameobject vector
    enemies.push_back(new enemy(ImVec2(posX, posY)));
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::addEnemy
* Description: Removies enemy from game and destroys it
* Input Parameters: enemy* e - enemy to be removed
* Return Value: void
*******************************************************************************************/
void gameObject::killEnemy(enemy* e) {
    if (enemies.size() > 0) {                                       //check there are actually enemies to remove
        for (auto a = enemies.begin(); a != enemies.end(); a++) {   //iterate through enemies
            if ((*a) == e) {                                        //if found, remove from vector and exit loop
                enemies.erase(a);
                break;
            }
        }
    }

    //update player stats and give reward
    this->p1->addKill();
    this->p1->addBalance(100);
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::addGun
* Description: Adds gun to player gun vector
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void gameObject::addGun(gun* g) {
    this->p1->addGun(g);
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gameObject::runTests
* Description: Runs Unit Tests
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void gameObject::runTests() {
    this->test->runTests();
}