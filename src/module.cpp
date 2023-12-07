#include "module.hpp"
#include <assert.h>
#include <cassert>
#include "math.h"

class moduleNode;

/*******************************************************************************************
*****************************          MODULE CLASS            *****************************
*******************************************************************************************/

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module (constructor)
* Description: Class Constructor for Module Class
* Input Parameters: position (ImVec2), rotation (float), pointer to parent (void*), pointer to gameObject (void*)
* Return Value: module instance
*******************************************************************************************/
module::module(ImVec2 pos, float rot, void* par, void* gaObj) {
    this->position = pos;
    this->health = 100.0f;
    this->rotation = rot;
    this->radius = 21.0f;
    this->parent = par;
    this->gObj = gaObj;
    for (int i = 0; i < 4; i++) {
        modNodes.push_back(new moduleNode());
        modNodes[ i ]->InitNodeModule(gaObj);
    }
}


/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: Module::setParent
* Description: Setter for the parent field of a module. Sets matching node to the parent module
                in moduleNodes vector to the pointer
* Input Parameters: idx (int)
* Return Value: boolean
*******************************************************************************************/
bool module::setParent(int idx) {
    assert(idx >= 0 && idx < 4);
    this->modNodes[ idx ]->setModuleNode(this->parent, idx);
    return 1;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::getGameObject
* Description: Getter for the pointer to the gameObject
* Input Parameters: N/A
* Return Value: void*
*******************************************************************************************/
void* module::getGameObject() {
    assert(this->gObj != NULL);
    return this->gObj;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::getGameObject
* Description: Getter for the modules node list
* Input Parameters: N/A
* Return Value: std::vector<moduleNode*>
*******************************************************************************************/
const std::vector<moduleNode*>& module::getModNodes() const {
    assert(this->modNodes.size() <= 4);
    return this->modNodes;
}
const std::vector<moduleNode*>& thruster::getModNodes() const {//thruster and guns need Null vector
    std::vector<moduleNode*> l;
    l.push_back(NULL);
    return l;
}
const std::vector<moduleNode*>& gun::getModNodes() const {//thruster and guns need Null vector
    std::vector<moduleNode*> l;
    l.push_back(NULL);
    return l;
}
/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::drawModule
* Description: Calls the moduleNodes draw function for each node in the module to draw the
                children to the screen
* Input Parameters: dl (ImDrawList*)
* Return Value: std::vector<moduleNode*>
               **Note: Virtual Function overwritten by all derived classes**
*******************************************************************************************/
bool module::drawModule(ImDrawList* dl) {
    assert(this->modNodes.size() >= 0);
    assert(dl != NULL);

    for (int i = 0; i < modNodes.size(); i++) {
        if ((modNodes[ i ]->getNodeModule() != NULL) && (modNodes[ i ]->getNodeModule() != this->parent)) {
            modNodes[ i ]->drawNodeModule(dl);
        }
    }
    return 1;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module destructor
* Description: Module class destructor. Removes the module from the parent's node list and
                deletes all of the module's children
* Input Parameters: N/A
* Return Value: Memory!!!
*******************************************************************************************/
module::~module() {
    ((module*)this->parent)->removeChild(this);
    for (int i = 0; i < modNodes.size(); i++) {
        if (modNodes[ i ]->getNodeModule() != NULL && modNodes[ i ]->getNodeModule() != this->parent)
            delete modNodes[ i ];
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::removeChild
* Description: Removes inputted module from the instances node list
* Input Parameters: m (module*)
* Return Value: boolean
*******************************************************************************************/
bool module::removeChild(module* m) {
    assert(m != NULL);
    for (int i = 0; i < modNodes.size(); i++) {
        if (modNodes[ i ]->getNodeModule() == m) {
            modNodes[ i ]->setModuleNode(NULL, i);
        }
    }

}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::showModuleNodes
* Description: recurses through the module's node list and calls the node's draw function
                for each node in the module to draw all available nodes. Also adds the ability
                to click on the module to open a popup menu to either sell or repair the module
* Input Parameters: dl (drawlist*), rd (float)(radius)
* Return Value: void
*******************************************************************************************/
void module::showModuleNodes(ImDrawList* dl, float rd) {
    //loop through child nodes and draw them
    for (int i = 0; i < this->modNodes.size(); i++) {
        modNodes[ i ]->showModuleNode(dl, rd, i, this);
    }

    //set the cursor position to the module's position for mouse hitbox
    ImGui::SetCursorPos(ImVec2(this->position.x - radius, this->position.y - radius));
    //push this instances pointer to the stack to be used in the popup menu
    ImGui::PushID(this);

    //detect if hitbox is clicked
    if (ImGui::InvisibleButton(" ", ImVec2(radius * 1.5f, radius * 1.5f))) {

        //open popup menu
        ImGui::OpenPopup("ModuleInfo", ImGuiPopupFlags_NoOpenOverExistingPopup);
    }

    //popup menu
    if (ImGui::BeginPopup("ModuleInfo")) {

        //popup menu options
        //sell the module
        if (ImGui::Selectable("Sell   +$100")) {
            ((gameObject*)(this->gObj))->getPlayer()->addBalance(100);      //return money back to the player
            ((gameObject*)(this->gObj))->getPlayer()->removeGun((gun*)this);//remove the gun from the players gunlist (even if not a gun the iterator still wont catch it)

            //Delete module and exit;
            delete this;
            ImGui::EndPopup();
            ImGui::PopID();
            return;

        }

        //repair module
        if (this->health < 100) {                                                                   // check if module is repairable(1)
            std::string repair = "Repair: -$" + std::to_string(100 - this->health);                 //create string for repair cost
            if (((gameObject*)(this->gObj))->getPlayer()->getBandCount() > (100 - this->health)) {  //check if player has enough money (2)
                if (ImGui::Selectable(repair.c_str())) {                                            //if repaired (3)
                    ((gameObject*)(this->gObj))->getPlayer()->addBalance(-(100 - this->health));    //subtract money from player
                    this->health = 100;                                                             //return module to full health
                }
            } else {                                            //if player does not have enough money (2)
                (ImGui::Text("Repair: Not Enough Money"));      //dont allow selectable and display not enough money
            }
        } else {                                                //if module is at full health (1)
            ImGui::Selectable("Fully Repaired");                //display full health
        }

        ImGui::EndPopup();                                      //end popup and remove from stack
    }
    ImGui::PopID();                                             //remove object from id stack
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::getParent
* Description: Getter for the pointer to the parent module
* Input Parameters: N/A
* Return Value: void*
*******************************************************************************************/
void* module::getParent() {
    return this->parent;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::decreaseHealth
* Description: Update the modules health and check if it is dead. If it is dead, remove it
                from the parent's node list and delete it
* Input Parameters: h (float) - hitpoints to be removed
* Return Value: void
*******************************************************************************************/
void module::decreaseHealth(float h) {
    //reduce health
    this->health -= h;
    //if dead remove from parents nodeList
    if (health <= 0) {
        ((module*)this->parent)->removeChild(this);
        //free memory
        delete this;
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::getPosition
* Description: Getter for the modules position
* Input Parameters: N/A
* Return Value: ImVec2
*******************************************************************************************/
ImVec2 module::getPosition() {
    return this->position;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::showModuleNodes
* Description: Updates modules position then updates the child nodes' positions
* Input Parameters: n_pos (ImVec2) - new position, rot - rotation(float)
* Return Value: void
*******************************************************************************************/
inline void module::updateModPosition(ImVec2& n_pos, float rot) {
    //update position
    this->position.x = n_pos.x;
    this->position.y = n_pos.y;
    this->rotation = rot;

    //update child nodes
    double T_rot = (M_PI / 2);          //rotation between nodes (90 degrees)
    float dist = this->radius * 1.5;    //distance from center of module to node
    static ImVec2 n;    //temp position
    for (int k = 0; k < modNodes.size(); k++) {//loop through module's nodes

        //if node is not empty or is not the parent, update the node's position
        if ((modNodes[ k ]->getNodeModule() == NULL) || (modNodes[ k ]->getNodeModule() != NULL && (modNodes[ k ]->getNodeModule() != this->parent))) {
            n.x = this->position.x - dist * cos(this->rotation + ((float)k * T_rot) + M_PI / 4);
            n.y = this->position.y - dist * sin(this->rotation + ((float)k * T_rot) + M_PI / 4);
            modNodes[ k ]->updateNodePosition(n, this->rotation);
        }
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: module::setNodeModule
* Description: sets the node at specified index to the inputted module
* Input Parameters: n_mod (void*)-new module, idx (int)-index
* Return Value: void
*******************************************************************************************/
void module::setNodeModule(void* n_mod, int idx) {
    //find directional matching index, ie parents node 2 = childs node 0
    int num = idx + ((idx > 1) ? -2 : 2);
    this->modNodes[ idx ]->setModuleNode(n_mod, num);
    ((module*)n_mod)->setParent(num);
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: truss::drawModule
* Description: draws the truss module
* Input Parameters: dl (ImDrawList*)
* Return Value: bool
*******************************************************************************************/
bool truss::drawModule(ImDrawList* dl) {
    //boolean for if module or children should be rendered first
    bool drawnChildren = false;

    //static array of points for the module
    static ImVec2 p[ 8 ];
    float sep = M_PI / 2.0f;

    //Variables
    float wMaxw = ImGui::GetWindowWidth();
    float wMaxh = ImGui::GetWindowHeight();
    ImVec2 windowCenter = ImVec2((wMaxw / 2.0f), (wMaxh / 2.0f));
    ImVec2 PlayerPos = ((gameObject*)(this->gObj))->getPlayer()->getPosition();

    //render backside module
    if ((modNodes[ 0 ]->getNodeModule() != NULL) && (modNodes[ 0 ]->getNodeModule() != this->parent)) {
        modNodes[ 0 ]->drawNodeModule(dl);
    }

    //Check player orientation to renter top or bottm first
    if ((PlayerPos.x * sin(this->rotation)) - ((PlayerPos.y * cos(this->rotation))) < 0) {
        //render bottom first
        if ((modNodes[ 1 ]->getNodeModule() != NULL) && (modNodes[ 1 ]->getNodeModule() != this->parent)) {
            modNodes[ 1 ]->drawNodeModule(dl);
        }
        drawnChildren = true;
    } else {
        //render back first
        if ((modNodes[ 2 ]->getNodeModule() != NULL) && (modNodes[ 2 ]->getNodeModule() != this->parent)) {
            modNodes[ 2 ]->drawNodeModule(dl);
        }
        if ((modNodes[ 3 ]->getNodeModule() != NULL) && (modNodes[ 3 ]->getNodeModule() != this->parent)) {
            modNodes[ 3 ]->drawNodeModule(dl);
        }

    }

    //calculate vertices
    for (int n = 0; n < 4;n++) {
        //calculate bottom square vertices
        p[ n ].x = this->position.x - (radius * cos(this->rotation + ((float)n * sep)));
        p[ n ].y = this->position.y - (radius * sin(this->rotation + ((float)n * sep)));
        //calculate top square vertices
        p[ n + 4 ].x = ((float)this->position.x + (-15.0f * (float)sin((((float)PlayerPos.x - ((float)(windowCenter.x))) / ((float)windowCenter.x)) - M_PI))) - (radius * cos(this->rotation + ((float)n * sep)));
        p[ n + 4 ].y = ((float)this->position.y + (-15.0f * (float)sin((((float)PlayerPos.y - ((float)(windowCenter.y))) / ((float)windowCenter.y)) - M_PI))) - (radius * sin(this->rotation + ((float)n * sep)));
    }

    //draw top square
    dl->AddQuad(p[ 0 ], p[ 1 ], p[ 2 ], p[ 3 ], ImColor(255, 255, 255, 210), 3.0f);
    dl->AddQuad(p[ 4 ], p[ 5 ], p[ 6 ], p[ 7 ], ImColor(255, 255, 255, 255), 3.0f);


    //draw virticle lines
    dl->AddLine(p[ 0 ], p[ 4 ], ImColor(255, 255, 255, 255), 3.0f);
    dl->AddLine(p[ 1 ], p[ 5 ], ImColor(255, 255, 255, 255), 3.0f);
    dl->AddLine(p[ 2 ], p[ 6 ], ImColor(255, 255, 255, 255), 3.0f);
    dl->AddLine(p[ 3 ], p[ 7 ], ImColor(255, 255, 255, 255), 3.0f);

    //render opposite children
    if (drawnChildren == false) {
        if ((modNodes[ 1 ]->getNodeModule() != NULL) && (modNodes[ 1 ]->getNodeModule() != this->parent)) {
            modNodes[ 1 ]->drawNodeModule(dl);
        }
    } else {
        if ((modNodes[ 2 ]->getNodeModule() != NULL) && (modNodes[ 2 ]->getNodeModule() != this->parent)) {
            modNodes[ 2 ]->drawNodeModule(dl);
        }
        if ((modNodes[ 3 ]->getNodeModule() != NULL) && (modNodes[ 3 ]->getNodeModule() != this->parent)) {
            modNodes[ 3 ]->drawNodeModule(dl);
        }
    }

    //render health bar
    if (this->health < 100) {
        ImVec2 healthBarPos = ImVec2(this->position.x, this->position.y - 10);
        dl->AddLine(ImVec2(healthBarPos.x - (health / 10), healthBarPos.y), ImVec2(healthBarPos.x + (health / 10), healthBarPos.y), ImColor(1 - (this->health / 100.0f), (this->health / 100.0f), 0.0f, 1.0f), 3.0f);
    }

    return 1;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gun::fireGun
* Description: fires a new bullet
* Input Parameters: bulletAngle (float)
* Return Value: void
*******************************************************************************************/
void gun::fireGun(float bulletAngle) {
    //create new bullet and add to gun vector
    pewpew->push_back(new bullet(this->position, bulletAngle, ImColor(255, 0, 0, 255)));
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: thruster::drawModule
* Description: draws the thruster module
* Input Parameters: dl (ImDrawList*)
* Return Value: bool
*******************************************************************************************/
bool thruster::drawModule(ImDrawList* dl) {
    //Variables
    float wMaxw = ImGui::GetWindowWidth();
    float wMaxh = ImGui::GetWindowHeight();
    ImVec2 windowCenter = ImVec2((wMaxw / 2.0f), (wMaxh / 2.0f));
    ImVec2 PlayerPos = ((gameObject*)(this->gObj))->getPlayer()->getPosition();
    static ImVec2 p[ 12 ];
    float sep = M_PI / 2.0f;
    //calculate vertices
    for (int n = 0; n < 4;n++) {
        //calculate cube bottem vertices
        p[ n ].x = this->position.x - (radius * cos(this->rotation + ((float)n * sep)));
        p[ n ].y = this->position.y - (radius * sin(this->rotation + ((float)n * sep)));
        //calculate cube top vertices
        p[ n + 4 ].x = ((float)this->position.x + (-15.0f * (float)sin((((float)PlayerPos.x - ((float)(windowCenter.x))) / ((float)windowCenter.x)) - M_PI))) - (radius * cos(this->rotation + ((float)n * sep)));
        p[ n + 4 ].y = ((float)this->position.y + (-15.0f * (float)sin((((float)PlayerPos.y - ((float)(windowCenter.y))) / ((float)windowCenter.y)) - M_PI))) - (radius * sin(this->rotation + ((float)n * sep)));
    }

    //calculate top of cone position
    p[ 8 ].x = p[ 7 ].x + ((p[ 2 ].x - p[ 7 ].x) / 2);
    p[ 8 ].y = p[ 7 ].y + ((p[ 2 ].y - p[ 7 ].y) / 2);

    //calculate bottom of cone base position
    p[ 9 ].x = p[ 5 ].x + ((p[ 1 ].x - p[ 5 ].x) / 2);
    p[ 9 ].y = p[ 5 ].y + ((p[ 1 ].y - p[ 5 ].y) / 2);

    //calculate top of cone base position
    p[ 10 ].x = p[ 4 ].x + ((p[ 0 ].x - p[ 4 ].x) / 2);
    p[ 10 ].y = p[ 4 ].y + ((p[ 0 ].y - p[ 4 ].y) / 2);

    //calculate 3d top of cone position for shading
    p[ 11 ].x = p[ 5 ].x + ((p[ 0 ].x - p[ 5 ].x) / 2);
    p[ 11 ].y = p[ 5 ].y + ((p[ 0 ].y - p[ 5 ].y) / 2);

    //if moving draw fire particles
    if (((gameObject*)(gObj))->getPlayer()->getIsMoving()) {
        for (int m = 0; m < 10;m++) {
            dl->AddCircleFilled(ImVec2(p[ 11 ].x - ((rand() % 20) - 10), p[ 11 ].y - ((rand() % 20) - 10)), rand() % 4, ImColor(255, 20, 20, rand() % 255));
        }
    }

    //draw cone
    if (rotation >= -M_PI / 4.0f) {//if facing down, draw left side lighter
        dl->AddTriangleFilled(p[ 8 ], p[ 9 ], p[ 11 ], ImColor(200, 90, 10, 255));
        dl->AddTriangleFilled(p[ 8 ], p[ 10 ], p[ 11 ], ImColor(255, 130, 40, 255));

    } else {//if facing up, draw left side lighter
        dl->AddTriangleFilled(p[ 8 ], p[ 10 ], p[ 11 ], ImColor(200, 90, 10, 255));
        dl->AddTriangleFilled(p[ 8 ], p[ 9 ], p[ 11 ], ImColor(255, 130, 40, 255));
        //if facing up, also draw inside of cone
        if (PlayerPos.y < windowCenter.y) {
            dl->AddEllipseFilled(p[ 11 ], 15.0f, (7.0f * cos(this->rotation - ((M_PI) / 4)) * (float)sin((((float)PlayerPos.y - ((float)(windowCenter.y))) / ((float)windowCenter.y)) - M_PI)), ImColor(80, 40, 10, 255), this->rotation - ((M_PI) / 4));
        }
    }
    //draw health bar
    if (this->health < 100) {
        ImVec2 healthBarPos = ImVec2(this->position.x, this->position.y - 10);
        dl->AddLine(ImVec2(healthBarPos.x - (health / 10), healthBarPos.y), ImVec2(healthBarPos.x + (health / 10), healthBarPos.y), ImColor(1 - (this->health / 100.0f), (this->health / 100.0f), 0.0f, 1.0f), 3.0f);
    }
    return 1;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: gun::drawModule
* Description: draws the gun module
* Input Parameters: dl (ImDrawList*)
* Return Value: bool
*******************************************************************************************/
bool gun::drawModule(ImDrawList* dl) {
    //Variables
    radius = 20;
    static ImVec2 p[ 6 ];
    float wMaxw = ImGui::GetWindowWidth();
    float wMaxh = ImGui::GetWindowHeight();
    ImVec2 windowCenter = ImVec2((wMaxw / 2.0f), (wMaxh / 2.0f));
    ImVec2 PlayerPos = ((gameObject*)(this->gObj))->getPlayer()->getPosition();
    float sep = M_PI / 2.0f;

    //calculate 3d offset
    float offsetX = (-15.0f * (float)sin((((float)PlayerPos.x - ((float)(windowCenter.x))) / ((float)windowCenter.x)) - M_PI)) / 2;
    float offsetY = (-15.0f * (float)sin((((float)PlayerPos.y - ((float)(windowCenter.y))) / ((float)windowCenter.y)) - M_PI)) / 2;

    //flip direction of gun depending on player orientation
    //calculate line connecting center of gun to parent module
    if (((position.x - PlayerPos.x) * sin(rotation)) - (((position.y - PlayerPos.y) * cos(rotation))) > 0) {

        p[ 0 ].x = this->position.x + offsetX - (radius * cos(this->rotation - (M_PI / 4.0f)));
        p[ 0 ].y = this->position.y + offsetY - (radius * sin(this->rotation - (M_PI / 4.0f)));
        p[ 1 ].x = this->position.x + offsetX - ((radius / 2) * cos(this->rotation - (M_PI / 4.0f)));
        p[ 1 ].y = this->position.y + offsetY - ((radius / 2) * sin(this->rotation - (M_PI / 4.0f)));
    } else {
        p[ 0 ].x = this->position.x + offsetX + (radius * cos(this->rotation - (M_PI / 4.0f)));
        p[ 0 ].y = this->position.y + offsetY + (radius * sin(this->rotation - (M_PI / 4.0f)));
        p[ 1 ].x = this->position.x + offsetX + ((radius / 2) * cos(this->rotation - (M_PI / 4.0f)));
        p[ 1 ].y = this->position.y + offsetY + ((radius / 2) * sin(this->rotation - (M_PI / 4.0f)));
    }

    //barrel verticies
    //verticle line
    p[ 2 ].x = p[ 1 ].x + offsetX;
    p[ 2 ].y = p[ 1 ].y + offsetY;
    p[ 3 ].x = p[ 1 ].x + offsetX;
    p[ 3 ].y = p[ 1 ].y - offsetY;
    //top barrel
    p[ 4 ].x = p[ 2 ].x + offsetX + (radius * cos(this->rotation + (M_PI / 4.0f)));
    p[ 4 ].y = p[ 2 ].y + (radius * sin(this->rotation + (M_PI / 4.0f)));
    //bottem barrel
    p[ 5 ].x = p[ 3 ].x + offsetX + ((radius * 0.85f) * cos(this->rotation + (M_PI / 4.0f)));
    p[ 5 ].y = p[ 3 ].y + ((radius * 0.85f) * sin(this->rotation + (M_PI / 4.0f)));

    //draw gun
    dl->AddLine(p[ 0 ], p[ 1 ], ImColor(160, 160, 160, 255), 2.0f);
    dl->AddLine(p[ 2 ], p[ 3 ], ImColor(160, 160, 160, 255), 2.0f);
    dl->AddLine(p[ 2 ], p[ 4 ], ImColor(160, 160, 160, 255), 4.0f);
    dl->AddLine(p[ 3 ], p[ 5 ], ImColor(160, 160, 160, 255), 4.0f);

    //draw health bar
    if (this->health < 100) {
        ImVec2 healthBarPos = ImVec2(this->position.x, this->position.y - 10);
        dl->AddLine(ImVec2(healthBarPos.x - (health / 10), healthBarPos.y), ImVec2(healthBarPos.x + (health / 10), healthBarPos.y), ImColor(1 - (this->health / 100.0f), (this->health / 100.0f), 0.0f, 1.0f), 3.0f);
    }
    return 1;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: thruster destructor
* Description: reduces the players speed when the thruster is destroyed
* Input Parameters: N/A
* Return Value: Memory!!!
*******************************************************************************************/
thruster::~thruster() {
    ((gameObject*)(this->gObj))->getPlayer()->addThruster(-2.5f, -5.0f);
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: thruster::thruster
* Description: Thruster constructor, disallows children and increases players speed
* Input Parameters:  pos (ImVec2) - position, rot (float)-rotation, par (void*) - parent module, g (void*) - gameobject pointer
* Return Value: N/A
*******************************************************************************************/
thruster::thruster(ImVec2 pos, float rot, void* par, void* g) :module(pos, rot, par, g) {
    this->modNodes.clear();
    ((gameObject*)(this->gObj))->getPlayer()->addThruster(2.5f, 5.0f);
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: moduleNode::moduleNode
* Description: moduleNode constructor, sets the child module pointer to NULL
* Input Parameters:  N/A
* Return Value: N/A
*******************************************************************************************/
moduleNode::moduleNode() {
    this->mod = NULL;
};

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: moduleNode::~moduleNode
* Description: moduleNode destructor, deletes child module
* Input Parameters:  N/A
* Return Value: Memory!!!
*******************************************************************************************/
moduleNode::~moduleNode() {
    if (this->mod != NULL) {
        delete this->mod;
        this->mod = NULL;
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: moduleNode::getNodeModule
* Description: returns child module if exists, otherwise returns null
* Input Parameters:  N/A
* Return Value: Memory!!!
*******************************************************************************************/
module* moduleNode::getNodeModule() {
    if (mod == NULL) {
        return NULL;
    } else {
        return mod;
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: moduleNode::getNodePosition
* Description: returns nodes position
* Input Parameters:  N/A
* Return Value: ImVec2
*******************************************************************************************/
ImVec2 moduleNode::getNodePosition() {
    return ModNodePos;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: moduleNode::InitNodeModule
* Description: Initializes node module, sets child module pointer to NULL, and sets gameobject pointer
* Input Parameters:  pGame(void*) - gameobject pointer
* Return Value: void
*******************************************************************************************/
void moduleNode::InitNodeModule(void* pGame) {
    this->mod = NULL;
    this->game = pGame;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: moduleNode::setModuleNode
* Description: Setter for child module pointer
* Input Parameters:  I_mod(void*) - new module, n(int) - index
* Return Value: void
*******************************************************************************************/
void moduleNode::setModuleNode(void* I_mod, int n) {
    this->mod = (module*)I_mod;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: moduleNode::drawNodeModule
* Description: Draws child module if it exists
* Input Parameters:  dl (ImDrawList*) - Imgui drawlist
* Return Value: void
*******************************************************************************************/
void moduleNode::drawNodeModule(ImDrawList* dl) {
    if (this->mod != NULL) {
        this->mod->drawModule(dl);
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: moduleNode::updateNodePosition
* Description: Updates node position and updates child module position
* Input Parameters:  pos (ImVec2) - position, rot (float) - rotation
* Return Value: void
*******************************************************************************************/
void moduleNode::updateNodePosition(ImVec2& pos, float rot) {
    //sets position
    ModNodePos.x = pos.x;
    ModNodePos.y = pos.y;
    //updates child module if exists
    if (this->mod != NULL) {
        this->mod->updateModPosition(ModNodePos, rot);
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: moduleNode::showModuleNode
* Description: For when player is in inventory, creates hitbox and displays popup for editing node
               if node is empty, otherwise traverses to next module
* Input Parameters: ImDrawList* dl - drawlist pointer, float rad - radius, int id - index,
                    module* parent - parent module
* Return Value: void
*******************************************************************************************/
void moduleNode::showModuleNode(ImDrawList* dl, float rad, int id, module* parent) {
    //if node is empty or "dead end"
    if (this->mod == NULL || this->mod->getParent() != parent) {
        dl->AddCircleFilled(this->ModNodePos, rad / 5, ImColor(255, 255, 255, 255));                //draw circle for node
        ImGui::SetCursorPos(ImVec2(this->ModNodePos.x - rad / 4, this->ModNodePos.y - rad / 5));    //set api cursor to node position-radius offset
        ImGui::PushID(this);                                                                        //push object to id stack
        if (ImGui::InvisibleButton(" ", ImVec2(rad / 2, rad / 2))) {                                //create hitbox for node
            ImGui::OpenPopup("NodeInfo", ImGuiPopupFlags_NoOpenOverExistingPopup);                  //if clicked, open popup for current id if none are already open
        }

        if (ImGui::BeginPopup("NodeInfo")) {                                                        //if popup should be open         
            int balance = ((gameObject*)(parent->getGameObject()))->getPlayer()->getBandCount();    //get player balance
            if (balance < 100) {                                                                    //if balance is too low to purchase thruster, display as such
                ImGui::Selectable("Buy Truss Module: Not Enough Money");
            } else {                                                                                //if suffecient funds, display option to add balance
                if (ImGui::Selectable("Buy Truss Module")) {
                    ((gameObject*)(parent->getGameObject()))->getPlayer()->addBalance(-100);        //if clicked, remove balance and add new module to opposite index
                    this->setModuleNode((void*)(new truss(this->ModNodePos, 0, (void*)parent, this->game)), ((id > 1) ? id - 2 : id + 2));
                    ((module*)this->mod)->setParent(((id > 1) ? id - 2 : id + 2));                  //set new modules parent to opposite index
                }
            }

            if (balance < 200) {                                                                    //If balance is too low to purchase thruster, display as such
                ImGui::Selectable("Buy Thruster Module: Not Enough Money");
            } else {                                                                                //if suffecient funds, display option to to purchace thruster
                if (ImGui::Selectable("Buy Thruster Module")) {
                    ((gameObject*)(parent->getGameObject()))->getPlayer()->addBalance(-200);        //if chosen, remove balance and add new module to opposite index
                    this->setModuleNode((void*)(new thruster(this->ModNodePos, 0, (void*)parent, this->game)), ((id > 1) ? id - 2 : id + 2));
                    ((module*)this->mod)->setParent(((id > 1) ? id - 2 : id + 2));
                }
            }

            if (balance < 300) {                                                                    //If balance is too low to purchase Gun module, display as such
                ImGui::Selectable("Buy Gun Module: Not Enough Money");
            } else {
                if (ImGui::Selectable("Buy Gun Module: $300")) {                                    //if suffecient funds, display option to to purchace gun
                    ((gameObject*)(parent->getGameObject()))->getPlayer()->addBalance(-300);        //if chosen, remove balance and add new module to opposite index
                    this->setModuleNode((void*)(new gun(this->ModNodePos, 0, (void*)parent, this->game)), ((id > 1) ? id - 2 : id + 2));
                    ((module*)this->mod)->setParent(((id > 1) ? id - 2 : id + 2));
                    ((gameObject*)this->game)->addGun((gun*)this->mod);                             //add gun to game object gun vector
                }
            }
            ImGui::EndPopup();  //end popup loop
        }
        ImGui::PopID();         //Remove object from id stack
        return;

    } else if (this->mod != NULL) {             //if node is not empty
        this->mod->showModuleNodes(dl, rad);    //traverse to next module
    }
}


/*******************************************************************************************
*****************************          BULLET CLASS            *****************************
*******************************************************************************************/

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: bullet::bullet
* Description: Bullet Constructor, sets bullet position, velocity, and color
* Input Parameters: N/A
* Return Value: N/A
*******************************************************************************************/
bullet::bullet() {
    len = 30;
    angle = 0;
    b_pos = ImVec2(0, 0);
    b_vel = ImVec2(0, 0);
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: bullet::bullet
* Description: Bullet Constructor, sets bullet position, velocity, and color
* Input Parameters:ImVec2& pos - position, float ang - player angle, ImColor col - color
* Return Value: N/A
*******************************************************************************************/
bullet::bullet(ImVec2& pos, float ang, ImColor col) {
    int speed = 17;
    len = 15;
    b_pos.x = pos.x;
    b_pos.y = pos.y;
    angle = ang;
    color = col;
    b_vel = ImVec2(speed * cos(angle), speed * sin(angle));
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: bullet::calcPosition
* Description: Update the bullet position
* Input Parameters: N/A
* Return Value: N/A
*******************************************************************************************/
void bullet::calcPosition() {
    //change in time
    float dt = ImGui::GetIO().DeltaTime * 50;

    //increase position
    b_pos.x += b_vel.x * dt;
    b_pos.y += b_vel.y * dt;
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: bullet::drawBullet
* Description: Draws the bullet
* Input Parameters: dl (ImDrawList*) - drawlist
* Return Value: N/A
*******************************************************************************************/
void bullet::drawBullet(ImDrawList* dl) {
    //create line from position to end of bullet in direction of travel
    ImVec2 p2 = ImVec2(0, 0);
    p2.x = b_pos.x + len * cos(angle);
    p2.y = b_pos.y + len * sin(angle);

    //Draw Bullet
    dl->AddLine(this->b_pos, p2, this->color);
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: bullet::distFromModule t *OverLoaded*
* Description: Traverses child modules and check collisions with them, calculates distance
                from input module to bullet, then returns flag
* Input Parameters: m (module*) - module pointer
* Return Value: float - flag (2.0f - collision with module, 1.0f - Out of game range (delete), 0.0f - no collision)
*******************************************************************************************/
float bullet::distFromModule(module* m) {
    //get modules child nodes
    std::vector<moduleNode*> mN = m->getModNodes();
    //traverse child nodes and check collisions
    for (int i = 0; i < mN.size(); i++) {
        if (mN[ i ]->getNodeModule() != NULL && mN[ i ]->getNodeModule() != m->getParent()) { //check not null and not going backwards up tree
            //get module position and recursivlly calculate distance
            this->distFromModule(mN[ i ]->getNodeModule());
        }
    }

    ImVec2 modPosition = ((module*)m)->getPosition();                                           //Get current module position
    double dist = sqrt(pow(b_pos.x - modPosition.x, 2) + pow(b_pos.y - modPosition.y, 2));      //calculate distance from bullet to module
    if (dist <= 24) {
        m->decreaseHealth(30);  //if collision with module, reduce module health
        return 2.0f;
    }
    if (dist > 3000) {  //out of range
        return 1.0f;
    } else {            //no collision
        return 0.0f;
    }
}

/*******************************************************************************************
* Programmer: Julian Lacey, Akash Rathnayaka , Adam El-Mobdy
* Class: CptS 122, Fall 2023; Lab Section 04
* Programming Assignment: PA9 - Final Project - Asteroids
* Date: December 4th, 2023
* Function: bullet::distFromModule t *OverLoaded*
* Description: classifies how to handle bullet, calculates distance from input position to bullet
* Input Parameters: ImVec2 modPos - module position
* Return Value: float - flag (2.0f - collision with module, 1.0f - Out of game range (delete), 0.0f - no collision)
* Returned distance origionally hence float, but should be enum or just int but didnt have time to change
*******************************************************************************************/
float bullet::distFromModule(ImVec2& modPos) {
    double dist = sqrt(pow(b_pos.x - modPos.x, 2) + pow(b_pos.y - modPos.y, 2));
    if (dist <= 24) {
        return 2.0f;
    } else if (dist > 3000) {
        return 1.0f;
    } else {
        return 0.0f;
    }
}

