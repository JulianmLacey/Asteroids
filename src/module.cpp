#include "module.hpp"
#include "math.h"



bullet::bullet(ImVec2& pos, float ang, ImColor col) {
    int speed = 17;
    len = 15;
    b_pos.x = pos.x;
    b_pos.y = pos.y;
    angle = ang;
    color = col;
    b_vel = ImVec2(speed * cos(angle), speed * sin(angle));
}

float bullet::distFromPlayer(ImVec2& playerPos) {
    double dist = sqrt(pow(b_pos.x - playerPos.x, 2) + pow(b_pos.y - playerPos.y, 2));
    if (dist <= 30) {
        std::cout << "bulletHit" << std::endl;
        return 2.0f;
    }
    if (dist > 3000) {
        return 1.0f;
    } else {
        return 0.0f;

    }
}

void bullet::calcPosition() {
    float dt = ImGui::GetIO().DeltaTime * 50;
    b_pos.x += b_vel.x * dt;
    b_pos.y += b_vel.y * dt;
}

void bullet::drawBullet(ImDrawList* dl) {
    //ImVec2 p2 = ImVec2(1000, 500);
    len += ((len <= 5.0) ? 5.0 : ImGui::GetIO().MouseWheel);
    ImVec2 p2 = ImVec2(0, 0);
    p2.x = b_pos.x + len * cos(angle);
    p2.y = b_pos.y + len * sin(angle);
    dl->AddLine(this->b_pos, p2, this->color);
}


module::module(ImVec2 pos, float rot, void* par, void* gaObj) {
    position = pos;
    rotation = rot;
    this->radius = 24;
    this->gObj = gaObj;
    //std::cout << "Setting Parent" << std::endl;

    this->parent = par;
    //std::cout << "Parent Set" << std::endl;
    for (int i = 0; i < 4; i++) {
        modNodes.push_back(new moduleNode());
        modNodes[ i ]->InitNodeModule(gaObj);
    }

}
void* module::getParent() {
    return this->parent;
}

void moduleNode::updateNodePosition(ImVec2& pos, float rot) {
    ModNodePos.x = pos.x;
    ModNodePos.y = pos.y;
    if (this->mod != NULL) {
        //ImGui::Text("updateNodePosition %lf ; %lf", ModNodePos.x, ModNodePos.y);
        this->mod->updateModPosition(ModNodePos, rot);
    }
}

inline void module::updateModPosition(ImVec2& n_pos, float rot) {
    this->position.x = n_pos.x;
    this->position.y = n_pos.y;
    this->rotation = rot;
    double T_rot = (M_PI / 2);
    float dist = this->radius * 1.5;
    static ImVec2 n;
    for (int k = 0; k < modNodes.size(); k++) {



        if ((modNodes[ k ]->getNodeModule() == NULL) || (modNodes[ k ]->getNodeModule() != NULL && (modNodes[ k ]->getNodeModule() != this->parent))) {
            n.x = this->position.x - dist * cos(this->rotation + ((float)k * T_rot) + M_PI / 4);
            n.y = this->position.y - dist * sin(this->rotation + ((float)k * T_rot) + M_PI / 4);
           //n.x = this->position.x - dist * cos(this->rotation + ((float)k * T_rot));
           //n.y = this->position.y - dist * sin(this->rotation + ((float)k * T_rot));

            modNodes[ k ]->updateNodePosition(n, this->rotation);
        }
        //ImGui::Text("updateModPosition %lf ; %lf", n_pos.x, n_pos.y);

       //std::cout << "Updating Module Positio\nX: " << position.x << " Y: " << position.y << std::endl;
    }
}

void module::drawModule(ImDrawList* dl) {
    for (int i = 0; i < modNodes.size(); i++) {
        if ((modNodes[ i ]->getNodeModule() != NULL) && (modNodes[ i ]->getNodeModule() != this->parent)) {
            modNodes[ i ]->drawNodeModule(dl);
        }

    }
    //dl->AddCircleFilled(position, 20, ImColor(0, 0, 255, 255));
    ImGui::Text("drawModule %lf ; %lf", position.x, position.y);

}


void moduleNode::drawNodeModule(ImDrawList* dl) {
    if (this->mod != NULL) {
        //dl->AddCircleFilled(this->ModNodePos, 25 / 5, ImColor(255, 255, 255, 255));
        this->mod->drawModule(dl);
    }
}


void moduleNode::showModuleNode(ImDrawList* dl, float rad, int id, module* parent) {
    if (this->mod == NULL || this->mod->getParent() != parent) {
        dl->AddCircleFilled(this->ModNodePos, rad / 5, ImColor(255, 255, 255, 255));

        ImGui::SetCursorPos(ImVec2(this->ModNodePos.x - rad / 4, this->ModNodePos.y - rad / 5));
        ImGui::PushID(this);


        if (ImGui::InvisibleButton(" ", ImVec2(rad / 2, rad / 2))) {
            std::cout << "Mouse Clicked Node" << std::endl;
            ImGui::OpenPopup("NodeInfo", ImGuiPopupFlags_NoOpenOverExistingPopup);
        }

        if (ImGui::BeginPopup("NodeInfo")) {
            //player* p = gameObject::getPlayer();
            int balance = ((gameObject*)(parent->getGameObject()))->getPlayer()->getBandCount();
            if (balance < 100) {
                ImGui::Selectable("Buy Truss Module: Not Enough Money");
            } else {
                if (ImGui::Selectable("Buy Truss Module")) {
                    ((gameObject*)(parent->getGameObject()))->getPlayer()->addBalance(-100);
                    this->setModuleNode((void*)(new truss(this->ModNodePos, 0, (void*)parent, this->game)), ((id > 1) ? id - 2 : id + 2));
                    ((module*)this->mod)->setParent(((id > 1) ? id - 2 : id + 2));
                }
            }
            if (balance < 200) {
                ImGui::Selectable("Buy Thruster Module: Not Enough Money");
            } else {
                if (ImGui::Selectable("Buy Thruster Module")) {
                    ((gameObject*)(parent->getGameObject()))->getPlayer()->addBalance(-200);
                    this->setModuleNode((void*)(new thruster(this->ModNodePos, 0, (void*)parent, this->game)), ((id > 1) ? id - 2 : id + 2));
                    ((module*)this->mod)->setParent(((id > 1) ? id - 2 : id + 2));
                }
            }


            if (balance < 300) {
                ImGui::Selectable("Buy Gun Module: Not Enough Money");
            } else {
                if (ImGui::Selectable("Buy Gun Module: $300")) {
                    ((gameObject*)(parent->getGameObject()))->getPlayer()->addBalance(-300);
                    this->setModuleNode((void*)(new gun(this->ModNodePos, 0, (void*)parent, this->game)), ((id > 1) ? id - 2 : id + 2));
                    ((module*)this->mod)->setParent(((id > 1) ? id - 2 : id + 2));
                    ((gameObject*)this->game)->addGun((gun*)this->mod);
                }
            }

            ImGui::EndPopup();
        }
        ImGui::PopID();

        return;

    } else if (this->mod != NULL) {
        this->mod->showModuleNodes(dl, rad);
    }

}
moduleNode::~moduleNode() {
    if (this->mod != NULL) {
        delete this->mod;
        ((gameObject*)game)->getPlayer()->addBalance(100);
        this->mod = NULL;
    }
}
module::~module() {
    ((module*)this->parent)->removeChild(this);
    for (int i = 0; i < modNodes.size(); i++) {

        delete modNodes[ i ];
    }
}
void module::removeChild(module* m) {
    for (int i = 0; i < modNodes.size(); i++) {
        if (modNodes[ i ]->getNodeModule() == m) {
            modNodes[ i ]->setModuleNode(NULL, i);
        }
    }
}
void module::showModuleNodes(ImDrawList* dl, float rd) {
    for (int i = 0; i < this->modNodes.size(); i++) {

        modNodes[ i ]->showModuleNode(dl, rd, i, this);

    }
    ImGui::SetCursorPos(ImVec2(this->position.x - radius / 4, this->position.y - radius / 5));
    ImGui::PushID(this);


    if (ImGui::InvisibleButton(" ", ImVec2(radius, radius))) {

        ImGui::OpenPopup("ModuleInfo", ImGuiPopupFlags_NoOpenOverExistingPopup);
    }

    if (ImGui::BeginPopup("ModuleInfo")) {

        if (ImGui::Selectable("Sell")) {
            ((gameObject*)(this->gObj))->getPlayer()->addBalance(100);
            ((gameObject*)(this->gObj))->getPlayer()->removeGun((gun*)this);

            ((module*)this->parent)->removeChild(this);


        }
        ImGui::EndPopup();
    }



    ImGui::PopID();
}

void moduleNode::setModuleNode(void* I_mod, int n) {

    this->mod = (module*)I_mod;

}

void module::setParent(int idx) {

    modNodes[ idx ]->setModuleNode(this->parent, idx);

}

void module::setNodeModule(void* n_mod, int idx) {
    int num = idx + ((idx > 1) ? -2 : 2);
    this->modNodes[ idx ]->setModuleNode(n_mod, num);
    ((module*)n_mod)->setParent(num);

}

void truss::drawModule(ImDrawList* dl) {
    for (int i = 0; i < modNodes.size(); i++) {
        if ((modNodes[ i ]->getNodeModule() != NULL) && (modNodes[ i ]->getNodeModule() != this->parent)) {
            modNodes[ i ]->drawNodeModule(dl);
        }
    }
    radius = 24;
    static ImVec2 p[ 4 ];
    float sep = M_PI / 2.0f;
    for (int n = 0; n < 4;n++) {
        p[ n ].x = this->position.x - (radius * cos(this->rotation + ((float)n * sep)));
        p[ n ].y = this->position.y - (radius * sin(this->rotation + ((float)n * sep)));
        //std::cout << "x: " << p[ n ].x << " y: " << p[ n ].y << std::endl;
    }
    //ImGui::Text("Points %lf ; %lf;%lf ; %lf", p[ 0 ].x, p[ 0 ].y, p[ 1 ].x, p[ 1 ].y);

    //std::cout << std::endl;
    dl->AddQuad(p[ 0 ], p[ 1 ], p[ 2 ], p[ 3 ], ImColor(255, 255, 255, 255), 0.5f);
}

// gun::gun(ImVec2 pos, float rot, player* par) :module(pos, rot, (void*)par) {
//     (par)->player::addGun(this);
// };
void gun::fireGun(float bulletAngle) {
    std::cout << "Firing Gun" << std::endl;
    pewpew->push_back(new bullet(this->position, bulletAngle, ImColor(255, 0, 0, 255)));
}
void thruster::drawModule(ImDrawList* dl) {
    for (int i = 0; i < modNodes.size(); i++) {
        if ((modNodes[ i ]->getNodeModule() != NULL) && (modNodes[ i ]->getNodeModule() != this->parent)) {
            modNodes[ i ]->drawNodeModule(dl);
        }
    }
    radius = 24;
    static ImVec2 p[ 4 ];
    float sep = M_PI / 2.0f;
    for (int n = 0; n < 4;n++) {
        p[ n ].x = this->position.x - (radius * cos(this->rotation + ((float)n * sep)));
        p[ n ].y = this->position.y - (radius * sin(this->rotation + ((float)n * sep)));
        //std::cout << "x: " << p[ n ].x << " y: " << p[ n ].y << std::endl;
    }
    dl->AddLine(p[ 1 ], p[ 2 ], ImColor(255, 130, 40, 255), 2.0f);
    dl->AddLine(p[ 2 ], p[ 3 ], ImColor(255, 130, 40, 255), 2.0f);
    dl->AddLine(p[ 0 ], p[ 3 ], ImColor(255, 130, 40, 255), 2.0f);

    if (((gameObject*)(gObj))->getPlayer()->getIsMoving()) {
        for (int m = 0; m < 10;m++) {
            dl->AddCircleFilled(ImVec2(this->position.x - ((rand() % 10) - 5), this->position.y - ((rand() % 20) - 10)), rand() % 7, ImColor(255, 130, 40, rand() % 255));
        }
    }

}
void gun::drawModule(ImDrawList* dl) {

    radius = 20;
    static ImVec2 p[ 4 ];

    p[ 0 ].x = this->position.x + (radius * (cos(this->rotation - (M_PI / 4.0f))));
    p[ 0 ].y = this->position.y + (radius * (sin(this->rotation - (M_PI / 4.0f))));
    p[ 1 ].x = this->position.x - (radius * (cos(this->rotation - (M_PI / 4.0f))));
    p[ 1 ].y = this->position.y - (radius * (sin(this->rotation - (M_PI / 4.0f))));

    dl->AddLine(p[ 0 ], p[ 1 ], ImColor(255, 0, 0, 255), 2.0f);
}

thruster::~thruster() {
    ((gameObject*)(this->gObj))->getPlayer()->addThruster(-2.5f, -5.0f);
}

thruster::thruster(ImVec2 pos, float rot, void* par, void* g) :module(pos, rot, par, g) {
    this->modNodes.clear();
    ((gameObject*)(this->gObj))->getPlayer()->addThruster(2.5f, 5.0f);
}
