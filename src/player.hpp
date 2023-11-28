#pragma once
#include "../imgui/imgui.h"


#include <iostream>
#include <math.h>



class bullet;
class enemy;
class module;
class gun;
class gameObject;

struct playerStatus {
    bool isMoving;
    bool isFiring;
    bool isInventory;
};

#include "module.hpp"
#include "gameobj.hpp"
class player : public module {

public:
    player() {};
    player(ImVec2 pos);
    ~player() {
        for (int i = 0; i < bullets.size(); i++) {
            delete bullets[ i ];
        }
    };
    int getBandCount();
    void drawPlayer();
    void CalculatePosition(ImVec2 accel);
    void update(std::vector<enemy*>& gameEnemies, gameObject* gObj);
    void fireBullet();
    void handleEvents();
    void showNodes();
    void addBalance(int m);
    int getKills();
    float getTimeAlive();
    void addGun(gun* g);
    //void setNodeModule(module* n_mod, int idx);
    ImDrawList* getPlayerDrawList();
    ImVec2 getPosition();
    ImVec2 getEnemy_PlayerHeading();
private:
    playerStatus curStatus;

    ImVec2 velocity;
    ImVec2 acc;

    float angle;
    int C_a = 5;
    float radius;
    float maxSpeed = 30;
    float friction = 0.15;
    uint balance;
    int kills;
    float timeAlive;
    ImDrawList* drawList;
    std::vector<bullet*> bullets;
    //std::vector<moduleNode*> modNodes;
    float headingDifferential;
    bool headingDifferentialSet;
    std::vector<gun*> pewpews;
};

class enemy : public player {
public:
    enemy() {};
    enemy(ImVec2 pos);
    ~enemy() {};
    void update(ImVec2& p1Position, ImVec2& p1PosReal);
    void calcEnemyPosition(ImVec2& p1Position);
    void drawEnemy(ImVec2& p1Position, ImDrawList* dl);
    float getTimeUntilNextShot();
    float distFromPlayer(ImVec2& playerPos);
    ImVec2 getPosition();
    void fireBullet();
private:
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
