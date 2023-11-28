#include "gameobj.hpp"
#include "time.h"


gameObject::gameObject(ImVec2 ScreenSize) {
    screenDIM = ScreenSize;
    p1 = new player(ImVec2(ScreenSize.x / 2, ScreenSize.y / 2));
    srand(time(NULL));
}

player* gameObject::getPlayer() {
    return p1;
}

void gameObject::updateGame() {
    p1->update(this->enemies, this);
    ImVec2 PlayerPosition = p1->getEnemy_PlayerHeading();
    ImVec2 PlayerPositionReal = p1->getPosition();
    if (enemies.size() >= 1) {

        for (auto a = enemies.begin(); a != enemies.end(); a++) {
            (*a)->update(PlayerPosition, PlayerPositionReal);
        }
    }
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
                delete (*a);
                enemies.erase(a);
                break;
            }
        }
    }
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
        cout << "Faild to start server !" << std::endl;
        return 0;
    }

    cout << "server started on port: " << port << std::endl;
    return 1;
}