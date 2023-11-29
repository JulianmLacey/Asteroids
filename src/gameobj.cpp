#include "gameobj.hpp"
#include "time.h"


gameObject::gameObject(ImVec2 ScreenSize) {
    screenDIM = ScreenSize;
    timeUntilNextEnemy = 0;
    maxEnemies = 2;
    level = 1;
    p1 = new player(ImVec2(ScreenSize.x / 2, ScreenSize.y / 2), this);
    srand(time(NULL));
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

player* gameObject::getPlayer() {
    return p1;
}
void gameObject::increaseLevel() {
    level++;
    if (level % 5 && level != 0) {
        maxEnemies++;
    }
}
void gameObject::drawBackground() {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    for (int i = 0; i < backgroundStars.size(); i++) {
        ImColor c = ImColor(1.0f, 1.0f, 1.0f, 1.0f * backgroundStars[ i ]->alpha);
        dl->AddCircleFilled(backgroundStars[ i ]->position, backgroundStars[ i ]->size, c);
    }
    for (int i = 0; i < foregroundStars.size(); i++) {
        ImColor c = ImColor(1.0f, 1.0f, 1.0f, 1.0f * foregroundStars[ i ]->alpha);
        dl->AddCircleFilled(foregroundStars[ i ]->position, foregroundStars[ i ]->size, c);
    }
}
void gameObject::updateGame() {

    p1->update(this->enemies, this);

    ImVec2 PlayerPosition = p1->getEnemy_PlayerHeading();
    ImVec2 PlayerPositionReal = p1->getPosition();



    if (!p1->getIsInventory()) {
        timeUntilNextEnemy -= ImGui::GetIO().DeltaTime;
        if (timeUntilNextEnemy <= 0 && enemies.size() < maxEnemies) {
            this->addEnemy();
            timeUntilNextEnemy = 10 / level;
        }
        if (enemies.size() >= 1) {

            for (auto a = enemies.begin(); a != enemies.end(); a++) {
                (*a)->update(PlayerPosition, PlayerPositionReal);
            }
        }
    }
    this->drawBackground();
}

void gameObject::addEnemy() {
    int posX = rand() % (int)screenDIM.x;
    int posY = rand() % (int)screenDIM.y;
    enemies.push_back(new enemy(ImVec2(posX, posY)));
}

void gameObject::killEnemy(enemy* e) {
    if (enemies.size() > 0) {
        for (auto a = enemies.begin(); a != enemies.end(); a++) {
            if ((*a) == e) {

                enemies.erase(a);


                break;
            }
        }
    }
    this->p1->addKill();
    this->p1->addBalance(100);
}

void gameObject::addGun(gun* g) {
    this->p1->addGun(g);
}
extern inline void printMSG(Node* nd, std::string msg) {
    std::cout << nd->ip << " : " << msg << std::endl;
}

bool gameObject::createServer() {
    int port = 12002;


    // create server instance with specified port number
    tcp::server server(port);

    // set async fn to handle incoming messages
    server.OnMessage(printMSG);

    //start listening to connections
    int result = server.Listen();

    if (result != 0) {
        std::cout << "Faild to start server !" << std::endl;
        return 0;
    }

    cout << "server started on port: " << port << std::endl;
    return 1;
}