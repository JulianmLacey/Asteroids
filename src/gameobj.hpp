#pragma once
#include "../imgui/imgui.h"

#include "tcp/tcp_server.hpp"
#include <sstream>

#include "module.hpp"
#include "player.hpp"

class gun;
class player;




class gameObject {

public:
    gameObject() {

    };
    gameObject(ImVec2 ScreenSize);
    ~gameObject() {
        for (int i = 0; i < enemies.size(); i++) {
            delete enemies[ i ];
        }
    };


    void updateGame();
    player* getPlayer();
    void addEnemy();
    void killEnemy(enemy* e);
    void addGun(gun* g);
    bool createServer();


private:
    player* p1;
    std::vector<enemy*> enemies;
    ImVec2 screenDIM;
};



