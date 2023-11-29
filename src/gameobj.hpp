#pragma once
#include "../imgui/imgui.h"

#include "tcp/tcp_server.hpp"
#include <sstream>

#include "module.hpp"
#include "player.hpp"

class gun;
class player;

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
    void increaseLevel();
    void drawBackground();
private:
    player* p1;
    std::vector<enemy*> enemies;
    std::vector<backgroundStar*> foregroundStars;
    std::vector<backgroundStar*> backgroundStars;
    ImVec2 screenDIM;
    float timeUntilNextEnemy;
    int level;
    int maxEnemies;
    tcp::server* server;
};



