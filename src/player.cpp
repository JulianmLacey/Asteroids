#include "player.hpp"
#include "math.h"
#include "gameobj.hpp"



/*******************************************************************************************
*****************************          PLAYER CLASS            *****************************
*******************************************************************************************/

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::player
* Description: Class Constructor for Player Class, sets default values for player
* Input Parameters: N/A
* Return Value: module instance
*******************************************************************************************/
player::player() {
    this->kills = 0;
    this->timeAlive = 0;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::player
* Description: Class Constructor for Player Class, sets default values for player
* Input Parameters: ImVec2 pos - initial position, gameObject* g - pointer to game object
* Return Value: module instance
*******************************************************************************************/
player::player(ImVec2 pos, gameObject* g) {
    //set default values
    position = ImVec2(600, 300);
    velocity = ImVec2(0, 0);
    radius = 18.5f;
    C_a = 5;
    maxSpeed = 30;
    friction = 0.15f;
    balance = 1000;
    timeAlive = 0.0f;
    health = 1000;
    headingDifferential = 0;
    headingDifferentialSet = false;
    kills = 0;
    this->parent = NULL;
    curStatus = { false,false,true };
    this->gObj = g;
    this->parent = NULL;

    //initialize nodes
    for (int i = 0; i < 4; i++) {
        modNodes.push_back(new moduleNode());
        modNodes[ i ]->InitNodeModule(this->gObj);
    }

    //ensure bullets are empty
    bullets.clear();
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::addBalance
* Description: Updates players balance
* Input Parameters: int m - amount to add to balance
* Return Value: void
*******************************************************************************************/
void player::addBalance(int m) {
    balance += m;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::drawPlayer
* Description: Draws player to screen and traverses module tree to draw all modules
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void player::drawPlayer() {
    //calculate angle of mouse from player
    if (!curStatus.isInventory) {
        angle = atan2((ImGui::GetMousePos().y - position.y), ImGui::GetMousePos().x - position.x) + 1;
    }

    //Variables
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    double T_rot = M_PI / 2.0f;
    double dist = this->radius * 1.3f;
    float wMaxw = ImGui::GetWindowWidth();
    float wMaxh = ImGui::GetWindowHeight();
    ImVec2 windowCenter = ImVec2((wMaxw / 2.0f), (wMaxh / 2.0f));

    //3D offset
    float offsetX = (((float)sin(((float)position.x - (float)windowCenter.x) / (float)windowCenter.x)));
    float offsetY = (((float)sin(((float)position.y - (float)windowCenter.y) / (float)windowCenter.y)));

    //Bottem side triangle
    ImVec2 p1((position.x - 18.4f * cosTheta), position.y - 18.4f * sinTheta + 1.0f);
    ImVec2 p2(position.x + offsetX - (1.2f * radius) * (cos(angle + (2 * M_PI / 3))), position.y - offsetY - (1.4f * radius) * sin(angle + (2 * M_PI / 3)));
    ImVec2 p3(position.x - 18.4f * (cos(angle - (2 * M_PI / 3))), position.y - 18.4f * sin(angle - (2 * M_PI / 3)) + 1.0f);
    ImVec2 p4, p5;

    //Top side Points
    p4.x = p1.x + (15.0f * offsetX);
    p4.y = p1.y + (15.0f * offsetY);

    p5.x = p3.x + (15.0f * offsetX);
    p5.y = p3.y + (15.0f * offsetY);

    //Calculate Nodes
    static ImVec2 n;
    if (angle - 1 >= 0) { //If facing down, draw child modules first
        for (int k = 0; k < 4; k++) {
            if (modNodes[ k ]->getNodeModule() == NULL) {//If node is empty, skip
                continue;
            }

            //calculate node position
            n.x = position.x - dist * cos(angle - ((float)k * T_rot) - M_PI / 3);
            n.y = position.y - dist * sin(angle - ((float)k * T_rot) - M_PI / 3);

            //update node position and draw child module
            modNodes[ k ]->updateNodePosition(n, angle - 1.85f);
            modNodes[ k ]->drawNodeModule(this->drawList);
        }

        //draw player
        drawList->AddQuadFilled(p1, p3, p5, p4, ImColor(255, 255, 255, 210));
        drawList->AddTriangleFilled(p2, p1, p4, ImColor(255, 255, 255, 245));
        drawList->AddTriangleFilled(p2, p3, p5, ImColor(255, 255, 255, 245));
    } else {//else facing down, draw player first  
        //draw player
        drawList->AddQuadFilled(p1, p3, p5, p4, ImColor(255, 255, 255, 210));
        drawList->AddTriangleFilled(p2, p1, p4, ImColor(255, 255, 255, 245));
        drawList->AddTriangleFilled(p2, p3, p5, ImColor(255, 255, 255, 245));

        //draw child modules
        for (int k = 0; k < 4; k++) {
            if (modNodes[ k ]->getNodeModule() == NULL) {
                continue;
            }
            n.x = position.x - dist * cos(angle - ((float)k * T_rot) - M_PI / 3);
            n.y = position.y - dist * sin(angle - ((float)k * T_rot) - M_PI / 3);

            modNodes[ k ]->updateNodePosition(n, angle - 1.85f);
            modNodes[ k ]->drawNodeModule(this->drawList);
        }
    }

    //Draw windshield
    drawList->AddTriangleFilled(p2, p4, p5, ImColor(150, 150, 150, 255));
    drawList->AddLine(p2, p4, ImColor(255, 255, 255, 255), 3.0f);
    drawList->AddLine(p2, p5, ImColor(255, 255, 255, 255), 3.0f);
    drawList->AddLine(p4, p5, ImColor(255, 255, 255, 255), 3.0f);
    return;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::CalculatePosition
* Description: Updates players position
* Input Parameters: ImVec2 accel - acceleration
* Return Value: void
*******************************************************************************************/
void player::CalculatePosition(ImVec2 accel) {
    float dT = ImGui::GetIO().DeltaTime * 10;   //Length of previous frame
    if (velocity.x < maxSpeed) {                //if player not at full speed, speed up
        velocity.x += (accel.x) * dT;
    }
    if (velocity.y < maxSpeed) {                //if player not at full speed, speed up
        velocity.y += (accel.y) * dT;
    }

    //Coasting effect
    if (velocity.x != 0) {                      //if player is not accelerating, slow them down
        velocity.x -= velocity.x * (friction * dT);
    }
    if (velocity.y != 0) {                      //if player is not accelerating, slow them down
        velocity.y -= velocity.y * (friction * dT);
    }

    //update position
    position.x += (velocity.x * dT) + ((accel.x) * dT * dT) / 2;
    position.y += (velocity.y * dT) + ((accel.y) * dT * dT) / 2;

    return;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::fireBullet
* Description: Fires a bullet from the player, and all guns attached to the player
* Input Parameters: ImVec2 accel - acceleration
* Return Value: void
*******************************************************************************************/
void player::fireBullet() {
    //add bullet to bullet vector
    bullets.push_back(new bullet(this->position, this->angle - 1, ImColor(255, 0, 0, 255)));

    //traverse guns and fire them
    for (int b = 0; b < pewpews.size(); b++) {
        pewpews[ b ]->fireGun(this->angle - 1);
    }

    return;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::showNodes
* Description: Draws the child nodes for when player is in inventory
* Input Parameters: N/A
* Return Value: void
* Note, Replaced by module::showModuleNodes and player::drawPlayer as we removed the ability
* to directly attach modules infront or to the side of the player, thus just starts traversing
* the module tree from the first module and could be depreciated
*******************************************************************************************/
void player::showNodes() {
    modNodes[ 0 ]->showModuleNode(this->drawList, this->radius, 0, this);
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::getKills
* Description: Getter for the player's kill count
* Input Parameters: N/A
* Return Value: int
*******************************************************************************************/
int player::getKills() {
    return this->kills;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::getTimeAlive
* Description: Getter for the player's Time Alive
* Input Parameters: N/A
* Return Value: float
*******************************************************************************************/
float player::getTimeAlive() {
    return this->timeAlive;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::removeGun
* Description: Removes gun module from players gun vector
* Input Parameters: gun *g - pointer to gun module to remove
* Return Value: void
*******************************************************************************************/
void player::removeGun(gun* g) {
    for (int i = 0; i < pewpews.size(); i++) {
        if (pewpews[ i ] == g) {
            pewpews.erase(pewpews.begin() + i);
            return;
        }
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::addGun
* Description: Adds a gun module to the player's gun vector
* Input Parameters: gun* g - pointer to new gun module
* Return Value: void
*******************************************************************************************/
void player::addGun(gun* g) {
    this->pewpews.push_back(g);
    g->setPewPew(&bullets);

}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::getIsMoving
* Description: Getter to query if player is moving
* Input Parameters: N/A
* Return Value: bool
*******************************************************************************************/
bool player::getIsMoving() {
    return curStatus.isMoving;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::getIsInventory
* Description: Getter to query if player is in inventory/paused
* Input Parameters: N/A
* Return Value: bool
*******************************************************************************************/
bool player::getIsInventory() {
    return curStatus.isInventory;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::addThruster
* Description: Updates players speed and acceleration
* Input Parameters: float da - change in acceleration, float ds - change in speed
* Return Value: void
*******************************************************************************************/
void player::addThruster(float da, float ds) {
    this->maxSpeed += ds;
    this->C_a += da;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::getHealth
* Description: Getter to query player's health
* Input Parameters: N/A
* Return Value: float
*******************************************************************************************/
float player::getHealth() {
    return this->health;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::getFirstModule
* Description: Getter for the first module in the player's module tree
* Input Parameters: N/A
* Return Value: module*
*******************************************************************************************/
module* player::getFirstModule() {
    return this->modNodes[ 0 ]->getNodeModule();
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::handleEvents
* Description: Handles IO inputs for the player
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void player::handleEvents() {
    //reset acceleration and status flags
    acc.x = 0;
    acc.y = 0;
    curStatus.isMoving = false;
    if (ImGui::IsKeyDown(ImGuiKey_W)) { //if W is pressed, accelerate up vertically
        acc.y -= C_a;
        curStatus.isMoving = true;
    }
    if (ImGui::IsKeyDown(ImGuiKey_A)) { //if A is pressed, accelerate left horizontally
        acc.x -= C_a;
        curStatus.isMoving = true;
    }
    if (ImGui::IsKeyDown(ImGuiKey_S)) { //if S is pressed, accelerate down vertically
        acc.y += C_a;
        curStatus.isMoving = true;
    }
    if (ImGui::IsKeyDown(ImGuiKey_D)) {//if D is pressed, accelerate right horizontally
        acc.x += C_a;
        curStatus.isMoving = true;
    }
    if (ImGui::IsKeyDown(ImGuiKey_E)) {//if E is pressed, open inventory: show nodes and dont continue to update
        this->showNodes();
        curStatus.isInventory = true;
        return;
    } else {//if not in inventory continue to update the game
        curStatus.isInventory = false;
        this->CalculatePosition(acc);

    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {    //on left mouse button release, fire bullet/s
        this->fireBullet();
        curStatus.isFiring = true;
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::getPlayerDrawList
* Description: Getter for the current window drawlist
* Input Parameters: N/A
* Return Value: ImDrawList*
*******************************************************************************************/
ImDrawList* player::getPlayerDrawList() {
    return this->drawList;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::update
* Description: Main Player update function, executes all player functionality
* Input Parameters: std::vector<enemy*> gameEnemies - game objects emeny vector,
                    gameObject* gObj - pointer to game object.
* Return Value: void
*******************************************************************************************/
void player::update(std::vector<enemy*>& gameEnemies, gameObject* gObj) {

    //Variables
    float dist;
    ImVec2 EnemyPositions;

    drawList = ImGui::GetWindowDrawList();  //get current drawlist
    this->handleEvents();                   //handle IO events - update movement
    this->drawPlayer();                     //draw the player

    if (!curStatus.isInventory) {                                       //if not in inventory, update bullets
        for (int i = 0; i < this->bullets.size(); i++) {
            this->bullets[ i ]->drawBullet(ImGui::GetWindowDrawList()); //Draw the bullet
            this->bullets[ i ]->calcPosition();                         //Update the bullet's position
            for (int j = 0;j < gameEnemies.size();j++) {                //check for collisions with enemies
                EnemyPositions = gameEnemies[ j ]->getPosition();       //Get enemy position
                dist = bullets[ i ]->distFromModule(EnemyPositions);    //Handle collision
                if (dist > 0) {                                         //If bullet is out of range or collision detected
                    bullet* temp = bullets[ i ];                        //Delete bullet and remove from vector
                    this->bullets.erase(this->bullets.begin() + i);
                    delete temp;

                }                                                       //If collision detected, kill enemy, and continue to next
                if (dist == 2.0) {
                    gObj->killEnemy(gameEnemies[ j ]);
                    j++;
                    return;
                }
            }
        }
    }

    //Every 10 kills the player moves to the next level or difficulty
    if (this->kills % 10 == 0 && this->kills != 0) {
        gObj->increaseLevel();
    }
}
ImVec2 player::getPosition() {
    return this->position;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::getEnemy_PlayerHeading
* Description: Creates a psuedo position of the player to be used by the enemies to calculate
                their heading/position
* Input Parameters: N/A
* Return Value: ImVec2
*******************************************************************************************/
ImVec2 player::getEnemy_PlayerHeading() {

    //alternate between adding and subtracting headingDifferential to create swaying behavior
    if ((headingDifferential >= 100) || headingDifferential <= -100) {
        headingDifferentialSet = headingDifferentialSet ? false : true;
    }
    if (headingDifferentialSet) {
        headingDifferential += ImGui::GetIO().DeltaTime * 15;
    } else {
        headingDifferential -= ImGui::GetIO().DeltaTime * 15;
    }

    //return player position +/- offset
    return ImVec2(position.x + headingDifferential, position.y + headingDifferential);
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::addKill
* Description: Updates the player's Kills
* Input Parameters: N/A
* Return Value: void
*******************************************************************************************/
void player::addKill() {
    this->kills++;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: player::getBandCount
* Description: Getter for the players balance
* Input Parameters: N/A
* Return Value: int
*******************************************************************************************/
int player::getBandCount() {
    return this->balance;
}

/*******************************************************************************************
*****************************           ENEMY CLASS            *****************************
*******************************************************************************************/

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: enemy::enemy
* Description: enemy constructor, sets default values for enemy
* Input Parameters: ImVec2 pos - initial position
* Return Value: enemy
*******************************************************************************************/
enemy::enemy(ImVec2 pos) {
    position = pos;
    velocity = ImVec2(0, 0);
    radius = 15;
    EnemyAngleDt = 0;
    timeUntilNextShot = 5;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: enemy::getTimeUntilNextShot
* Description: Getter for the time until the enemy can shoot again
* Input Parameters: N/A
* Return Value: float
*******************************************************************************************/
float enemy::getTimeUntilNextShot() {
    return timeUntilNextShot;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: enemy::update
* Description: Enemy update function, updates enemy behaviour and draws enemy to screen
* Input Parameters: ImVec2 p1Position - player pseudo position, player p1 - player object
* Return Value: void
*******************************************************************************************/
void enemy::update(ImVec2& p1Position, player& p1) {
    //Variables
    ImVec2 p1PosReal = p1.getPosition();                              //get player position
    ImDrawList* dl = ImGui::GetWindowDrawList();                      //get current drawlist
    this->calcEnemyPosition(p1Position);                              //update enemy position
    this->drawEnemy(p1Position, dl);                                  //draw enemy to screen
    timeUntilNextShot -= ImGui::GetIO().DeltaTime * ((rand() % 5));   //update time until next shot
    module* firstMod = p1.getFirstModule();                           //get root of player module tree
    float distE = 0;                                                  //temp distance variable
    //If emeny can shoot and player in range
    if (timeUntilNextShot <= 0 && this->distFromPlayer(p1PosReal) < 600) {
        this->fireBullet();         //shoot
        timeUntilNextShot = 5;      //reset time until next shot
    }

    //check enemy bullet collisions
    for (int i = 0; i < EnemyBullets.size(); i++) {
        EnemyBullets[ i ]->calcPosition();                              //update bullet position
        distE = EnemyBullets[ i ]->bullet::distFromModule(p1PosReal);   //check for collision
        if (distE == 2.0f) {                               //if collision with player, exit program
            exit(0);
        }
        if (distE) {
            EnemyBullets.erase(EnemyBullets.begin() + i);               //if out of bounds, delete bullet
        } else {
            EnemyBullets[ i ]->drawBullet(ImGui::GetWindowDrawList());  //otherwise draw bullet and check other modules recursively
            this->bulletCollisions(firstMod, i);
        }
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: enemy::bulletCollisions
* Description: recursive function to check bullet collisions with player
* Input Parameters: module* m - current module, int bullID - current bullet index
* Return Value: void
*******************************************************************************************/
void enemy::bulletCollisions(module* m, int bullID) {
    //Variables
    float dist = 0;
    ImVec2 tempPlayerPosition;
    const std::vector<moduleNode*> pNodes = m->getModNodes();
    tempPlayerPosition = m->getPosition();                              //get module position
    dist = EnemyBullets[ bullID ]->distFromModule(tempPlayerPosition);  //check for collision
    if (dist == 2.0) {                                                  //if collision detected, adjust module health
        m->decreaseHealth(30.0f);
    }
    if (dist) {                                                         //if out of bounds, delete bullet
        EnemyBullets.erase(EnemyBullets.begin() + bullID);
    } else {                                                            //otherwise draw bullet
        EnemyBullets[ bullID ]->drawBullet(ImGui::GetWindowDrawList());
    }

    //if module has no children, return
    if (pNodes.size() != 4)
        return;

    //check all child nodes
    for (int j = 0; j < 4; j++) {
        if (pNodes[ j ] != NULL && (pNodes[ j ]->getNodeModule() != NULL && pNodes[ j ]->getNodeModule()->getParent() != m)) {
            this->bulletCollisions(pNodes[ j ]->getNodeModule(), bullID);
        }
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: enemy::distFromPlayer
* Description: Get enemy distance from player
* Input Parameters: ImVec2& playerPos - position of player
* Return Value: void
*******************************************************************************************/
float enemy::distFromPlayer(ImVec2& playerPos) {
    double dist = sqrt(pow(position.x - playerPos.x, 2) + pow(position.y - playerPos.y, 2));
    return dist;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: enemy::getPosition
* Description: Getter for enemy position
* Input Parameters: N/A
* Return Value: ImVec2
*******************************************************************************************/
ImVec2 enemy::getPosition() {
    return position;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: enemy::fireBullet
* Description: Adds a bullet to the enemy bullet vector
* Input Parameters: N/A
* Return Value: ImVec2
*******************************************************************************************/
void enemy::fireBullet() {
    EnemyBullets.push_back(new bullet(this->position, this->EnemyAngle - 1, ImColor(255, 233, 0, 255)));
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: enemy::drawEnemy
* Description: Draws Enemy to screen
* Input Parameters: ImVec2& p1Position - player position, ImDrawList* dl - drawlist
* Return Value: void
*******************************************************************************************/
void enemy::drawEnemy(ImVec2& p1Position, ImDrawList* dl) {
    //Calculate enemy heading angle
    EnemyAngle = atan2((p1Position.y - position.y), p1Position.x - position.x) + 1;
    if (EnemyAngle - EnemyAngleDt > 0.01) {
        EnemyAngle -= 0.01;
    }
    if (EnemyAngle - EnemyAngleDt < -0.01) {
        EnemyAngle += 0.01;
    }
    EnemyAngleDt = EnemyAngle;

    //Variables
    float cosTheta = cos(EnemyAngle);
    float sinTheta = sin(EnemyAngle);

    //calculate player triangle window location
    ImVec2 ep1((position.x - radius * cosTheta), position.y - radius * sinTheta);
    ImVec2 ep2(position.x - radius * (cos(EnemyAngle + (2 * M_PI / 3))), position.y - radius * sin(EnemyAngle + (2 * M_PI / 3)));
    ImVec2 ep3(position.x - radius * (cos(EnemyAngle - (2 * M_PI / 3))), position.y - radius * sin(EnemyAngle - (2 * M_PI / 3)));

    //draw enemy
    dl->AddTriangleFilled(ep1, ep2, ep3, ImColor(200, 0, 0, 255));
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: enemy::calcEnemyPosition
* Description: Updates Enemy Position
* Input Parameters: ImVec2& p1Position - player position
* Return Value: void
*******************************************************************************************/
void enemy::calcEnemyPosition(ImVec2& p1Position) {
    float dT = ImGui::GetIO().DeltaTime * 10;
    ImVec2 accel = ImVec2(acc * cos(EnemyAngle), acc * sin(EnemyAngle));
    if (velocity.x < maxSpeed) {
        velocity.x += (accel.x) * dT;
    }
    if (velocity.y < maxSpeed) {
        velocity.y += (accel.y) * dT;
    }

    if (velocity.x != 0) {
        velocity.x -= velocity.x * (friction * dT);
    }
    if (velocity.y != 0) {
        velocity.y -= velocity.y * (friction * dT);
    }
    position.x += (velocity.x * dT) + ((accel.x) * dT * dT) / 2;
    position.y += (velocity.y * dT) + ((accel.y) * dT * dT) / 2;
}