#pragma once
#include "../imgui/imgui.h"


#include <iostream>


class gameObject;
class moduleNode;
class bullet;

class module {
public:
    module() {};
    module(ImVec2 pos, float rot, void* par, void* gaObj);
    ~module();

    virtual void updateModPosition(ImVec2& n_pos, float rot);
    virtual void drawModule(ImDrawList* dl);
    void showModuleNodes(ImDrawList* dl, float rd);
    void setNodeModule(void* n_mod, int idx);
    void setParent(int idx);
    void* getParent();
    void* getGameObject() {
        return this->gObj;
    }
    void removeChild(module* m);
protected:
    ImVec2 position;
    float mass;
    float rotation;
    std::vector<moduleNode*> modNodes;
    float radius;
    void* parent;
    void* gObj;
};


class truss : public module {
public:
    truss() :module() {};
    truss(ImVec2 pos, float rot, void* par, void* g) :module(pos, rot, par, g) {};
    ~truss() {};
    void drawModule(ImDrawList* dl) override;

private:

};

class thruster : public module {
public:
    thruster() :module() {
        this->modNodes.clear();
    };
    thruster(ImVec2 pos, float rot, void* par, void* g);
    ~thruster();
    void drawModule(ImDrawList* dl) override;

private:

};
class gun : public module {
public:
    gun() :module() { this->modNodes.clear(); };
    gun(ImVec2 pos, float rot, void* par, void* g) :module(pos, rot, par, g) { this->modNodes.clear(); };
    ~gun();
    void drawModule(ImDrawList* dl) override;
    void fireGun(float bulletAngle);
    void setPewPew(std::vector<bullet*>* pew) {
        this->pewpew = pew;
    }
private:
    std::vector<bullet*>* pewpew;
};
#include "gameobj.hpp"
class moduleNode {
public:
    moduleNode() {
        mod = NULL;
    };

    ~moduleNode();
    void updateNodePosition(ImVec2& pos, float rot);
    void drawNodeModule(ImDrawList* dl);
    void showModuleNode(ImDrawList* dl, float rad, int id, module* parent);

    module* getNodeModule() {
        if (mod == NULL) {
            return NULL;
        } else {
            return mod;
        }
    };
    ImVec2 getNodePosition() {
        return ModNodePos;
    }
    void setModuleNode(void* I_mod, int n);
    void InitNodeModule(void* pGame) {
        this->mod = NULL;
        this->game = pGame;
    }
private:
    module* mod;
    void* game;
    ImVec2 ModNodePos;
};

class bullet {
public:
    bullet() {
        len = 30;
        angle = 0;
        b_pos = ImVec2(0, 0);
        b_vel = ImVec2(0, 0);
    };
    bullet(ImVec2& pos, float ang, ImColor col);
    ~bullet() {};

    void drawBullet(ImDrawList* dl);
    float distFromPlayer(ImVec2& playerPos);
    void calcPosition();
    void fireBullet();
private:
    ImVec2 b_pos;
    ImVec2 b_vel;
    float len;
    float angle;
    ImColor color;

};


