/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka, Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Description: The module class is the base class for all modules in the game. The structure
                of the ship is a double linked tree in which each module has a list of nodes,
                of which child moduled are attached to. Each module also contains the parent
                in its node list to stop the traversal up the tree.
*******************************************************************************************/
#pragma once
#include "../imgui/imgui.h"
#include <iostream>

//Forward declarations
class gameObject;
class moduleNode;
class bullet;

class module {
public:

//Constructors and destructor
    module() {};
    module(ImVec2 pos, float rot, void* par, void* gaObj);
    ~module();

    //Setters and Getters
    void decreaseHealth(float h);                              //Updates the module's health
    bool setParent(int idx);                                   //Sets the modules Parent
    void* getParent();                                         //Returns the modules parent
    void* getGameObject();                                     //Returns the game object
    ImVec2 getPosition();                                      //Returns the module's position
    bool removeChild(module* m);                               //Removes a child from the module    
    const std::vector<moduleNode*>& getModNodes() const;                    //Returns the module's nodes
    virtual bool drawModule(ImDrawList* dl);                   //Draws the module
    void setNodeModule(void* n_mod, int idx);                  //Sets A modules node to another module
    void showModuleNodes(ImDrawList* dl, float rd);            //Draws the Nodes of the module
    virtual void updateModPosition(ImVec2& n_pos, float rot);  //Updates the module's position


protected:

    ImVec2 position;
    float health;
    float rotation;
    std::vector<moduleNode*> modNodes;
    float radius;
    void* parent;
    void* gObj;
};

/*-----------------------------Module Types-----------------------------*/
class truss : public module {
public:
    truss() :module() {};
    truss(ImVec2 pos, float rot, void* par, void* g) :module(pos, rot, par, g) {};
    ~truss() {};
    bool drawModule(ImDrawList* dl) override;
private:
};

class thruster : public module {
public:
    thruster() :module() {
        this->modNodes.clear();
    };
    thruster(ImVec2 pos, float rot, void* par, void* g);
    const std::vector<moduleNode*>& getModNodes() const;
    ~thruster();
    bool drawModule(ImDrawList* dl) override;
private:
};

class gun : public module {
public:
    gun() :module() { this->modNodes.clear(); };
    gun(ImVec2 pos, float rot, void* par, void* g) :module(pos, rot, par, g) { this->modNodes.clear(); };
    ~gun();
    const std::vector<moduleNode*>& getModNodes() const;
    bool drawModule(ImDrawList* dl) override;
    void fireGun(float bulletAngle);
    void setPewPew(std::vector<bullet*>* pew) {
        this->pewpew = pew;
    }
private:
    std::vector<bullet*>* pewpew;
};

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka, Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Description: The moduleNodes provides the main functionality for the modules to be connected
                to each other. The moduleNode class is used to store the module that is connected
                to it and the position of the node on the module. The moduleNode class also
                contains the functionality to draw the node and the module that is connected to
                it, to update the position of the node and the module.
*******************************************************************************************/
#include "gameobj.hpp"
class moduleNode {
public:
    moduleNode();
    ~moduleNode();

    //Getters and Setters
    module* getNodeModule();                                                //Returns the Node's linked module
    ImVec2 getNodePosition();                                               //Returns the Node's position
    void InitNodeModule(void* pGame);                                       //Initializes the Node's linked module (sets the module pointer to NULL)
    void drawNodeModule(ImDrawList* dl);                                    //Draws the Node's linked module
    void setModuleNode(void* I_mod, int n);                                 //Sets the Node's linked module
    void updateNodePosition(ImVec2& pos, float rot);                        //Updates the Node's position
    void showModuleNode(ImDrawList* dl, float rad, int id, module* parent); //Draws the Node

private:
    void* game;
    module* mod;
    ImVec2 ModNodePos;
};

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka, Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Description: The Bullet Class is used to store the data for the bullets that are fired
                from the player or gun modules. The bullet class contains the functionality to draw
                the bullet, calculate the position of the bullet, and to fire the bullet.
*******************************************************************************************/
class bullet {
public:
    bullet();
    bullet(ImVec2& pos, float ang, ImColor col);
    ~bullet() {};

    void calcPosition();                        //Updates the position of the bullet
    void drawBullet(ImDrawList* dl);            //Draws the bullet   
    float distFromModule(module* m);            //Calculates the distance from a module or point (Recursive - parses whole tree)
    float distFromModule(ImVec2& modPos);       //Calculates the distance from a module or point
private:
    ImVec2 b_pos;
    ImVec2 b_vel;
    float len;
    float angle;
    ImColor color;
};


