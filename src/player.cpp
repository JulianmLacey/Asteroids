#include "player.hpp"
#include "math.h"
#include "gameobj.hpp"


player::player(ImVec2 pos) {
    position = ImVec2(600, 300);
    velocity = ImVec2(0, 0);
    radius = 20;
    balance = 0;
    timeAlive = 0;
    headingDifferential = 0;
    headingDifferentialSet = false;
    curStatus = { false,false,false };

    for (int i = 0; i < 4; i++) {
        modNodes.push_back(new moduleNode());
        modNodes[ i ]->InitNodeModule(this->gObj);
    }

}

void player::addBalance(int m) {
    balance += m;
}
void player::drawPlayer() {
    //x* cosTheta - y * sinTheta

    //calculate angle of mouse from player
    if (!curStatus.isInventory) {
        angle = atan2((ImGui::GetMousePos().y - position.y), ImGui::GetMousePos().x - position.x) + 1;
    }
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);


    //-(A*Math.cos(angle),A*Math.sin(angle))
    // -(A*Math.cos(angle+2*Math.PI/3),A*Math.sin(angle+2*Math.PI/3))
    // -(A*Math.cos(angle-2*Math.PI/3),A*Math.sin(angle-2*Math.PI/3))

    //calculate player triangle window location
    ImVec2 p1((position.x - radius * cosTheta), position.y - radius * sinTheta);
    ImVec2 p2(position.x - radius * (cos(angle + (2 * M_PI / 3))), position.y - radius * sin(angle + (2 * M_PI / 3)));
    ImVec2 p3(position.x - radius * (cos(angle - (2 * M_PI / 3))), position.y - radius * sin(angle - (2 * M_PI / 3)));


    //draw player triangle

    drawList->AddTriangle(p1, p2, p3, ImColor(0, 0, 255, 255));

    double T_rot = M_PI / 2;

    double dist = this->radius * 1.5;

    static ImVec2 n;
    for (int k = 0; k < 4; k++) {
        n.x = position.x - dist * cos(angle - ((float)k * T_rot) - M_PI / 3);
        n.y = position.y - dist * sin(angle - ((float)k * T_rot) - M_PI / 3);



        modNodes[ k ]->updateNodePosition(n, angle - 1.85f);

        modNodes[ k ]->drawNodeModule(this->drawList);


    }



    //show radius circle
   // drawList->AddCircle(position, radius, ImColor(0, 0, 255, 255));

}


void player::CalculatePosition(ImVec2 accel) {
    float dT = ImGui::GetIO().DeltaTime * 10;
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

void player::fireBullet() {
    bullets.push_back(new bullet(this->position, this->angle - 1, ImColor(255, 0, 0, 255)));
    for (int b = 0; b < pewpews.size(); b++) {
        pewpews[ b ]->fireGun(this->angle - 1);
    }
}

void player::showNodes() {

    float cosTheta = cos(this->angle);
    float sinTheta = sin(this->angle);

    double rot = M_PI / 2;

    double dist = this->radius * 1.5;
    ImVec2 n;

    n.x = position.x - dist * cos(angle);
    n.y = position.y - dist * sin(angle);

    //drawList->AddCircleFilled(n, this->radius / 5, ImColor(168, 197, 7, 193));
    modNodes[ 0 ]->showModuleNode(this->drawList, this->radius, 0, this);

}
int player::getKills() {
    return this->kills;
}
float player::getTimeAlive() {
    return this->timeAlive;
}
void player::addGun(gun* g) {
    this->pewpews.push_back(g);
    g->setPewPew(&bullets);

}
void player::handleEvents() {

    acc.x = 0;
    acc.y = 0;
    curStatus.isMoving = false;
    if (ImGui::IsKeyDown(ImGuiKey_W)) {
        acc.y -= C_a;
        curStatus.isMoving = true;
    }
    if (ImGui::IsKeyDown(ImGuiKey_A)) {
        acc.x -= C_a;
        curStatus.isMoving = true;

    }
    if (ImGui::IsKeyDown(ImGuiKey_S)) {
        acc.y += C_a;
        curStatus.isMoving = true;

    }
    if (ImGui::IsKeyDown(ImGuiKey_D)) {
        acc.x += C_a;
        curStatus.isMoving = true;

    //std::cout << "D Pressed" << std::endl;
    }
    if (ImGui::IsKeyDown(ImGuiKey_E)) {
        this->showNodes();
        curStatus.isInventory = true;
        return;
        //std::cout << "E Pressed" << std::endl;
    } else {
        curStatus.isInventory = false;
        this->CalculatePosition(acc);

    }

    //if ImGui::IsKeyReleased(ImGuiKey_Space))
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        this->fireBullet();
        curStatus.isFiring = true;
    }

    //this->radius += ((this->radius <= 6.0) ? 6.0 : ImGui::GetIO().MouseWheel);


}
ImDrawList* player::getPlayerDrawList() {
    return this->drawList;
}

void player::update(std::vector<enemy*>& gameEnemies, gameObject* gObj) {
    drawList = ImGui::GetWindowDrawList();
    this->handleEvents();

    this->drawPlayer();
    float dist;
    ImVec2 EnemyPositions;
    if (!curStatus.isInventory) {
        for (int i = 0; i < bullets.size(); i++) {
            bullets[ i ]->calcPosition();
            for (int j = 0;j < gameEnemies.size();j++) {
                EnemyPositions = gameEnemies[ j ]->getPosition();
                dist = bullets[ i ]->distFromPlayer(EnemyPositions);
                if (dist == 2.0) {
                    gObj->killEnemy(gameEnemies[ j ]);
                    j++;
                } else if (dist) {
                    bullets.erase(bullets.begin() + i);
                    delete bullets[ i ];
                    i++;
                }
            }
            bullets[ i ]->drawBullet(ImGui::GetWindowDrawList());
        }
    }
}
ImVec2 player::getPosition() {
    return this->position;
}
ImVec2 player::getEnemy_PlayerHeading() {

    if ((headingDifferential >= 100) || headingDifferential <= -100) {
        headingDifferentialSet = headingDifferentialSet ? false : true;
    }
    if (headingDifferentialSet) {
        headingDifferential += ImGui::GetIO().DeltaTime * 15;
    } else {
        headingDifferential -= ImGui::GetIO().DeltaTime * 15;
    }
    //std::cout << headingDifferential << std::endl;
    return ImVec2(position.x + headingDifferential, position.y + headingDifferential);

}
int player::getBandCount() {
    return this->balance;
}
//-----------ENEMY CLASS------------------//
enemy::enemy(ImVec2 pos) {
    position = pos;
    velocity = ImVec2(0, 0);
    radius = 15;
    EnemyAngleDt = 0;
    timeUntilNextShot = 5;
}

float enemy::getTimeUntilNextShot() {
    return timeUntilNextShot;
}

void enemy::update(ImVec2& p1Position, ImVec2& p1PosReal) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    this->calcEnemyPosition(p1Position);
    this->drawEnemy(p1Position, dl);


    timeUntilNextShot -= ImGui::GetIO().DeltaTime * ((rand() % 5));

    if (timeUntilNextShot <= 0 && this->distFromPlayer(p1PosReal) < 600) {
        this->fireBullet();
        timeUntilNextShot = 5;
    }
    float distE;
    for (int i = 0; i < EnemyBullets.size(); i++) {
        EnemyBullets[ i ]->calcPosition();
        distE = EnemyBullets[ i ]->distFromPlayer(p1PosReal);
        if (distE == 2.0) {
            std::cout << "Player Hit" << std::endl;
        } else if (distE) {
            std::cout << "Enemy Bullet OutOfRange" << std::endl;
            delete EnemyBullets[ i ];
            EnemyBullets.erase(EnemyBullets.begin() + i);
            i++;
        } else {
            EnemyBullets[ i ]->drawBullet(ImGui::GetWindowDrawList());
        }
    }
}


float enemy::distFromPlayer(ImVec2& playerPos) {
    double dist = sqrt(pow(position.x - playerPos.x, 2) + pow(position.y - playerPos.y, 2));
    return dist;
}
ImVec2 enemy::getPosition() {
    return position;
}
void enemy::fireBullet() {
    EnemyBullets.push_back(new bullet(this->position, this->EnemyAngle - 1, ImColor(255, 233, 0, 255)));
    std::cout << EnemyBullets.size() << std::endl;
}

void enemy::drawEnemy(ImVec2& p1Position, ImDrawList* dl) {
    EnemyAngle = atan2((p1Position.y - position.y), p1Position.x - position.x) + 1;
    if (EnemyAngle - EnemyAngleDt > 0.01) {
        EnemyAngle -= 0.01;
    }
    if (EnemyAngle - EnemyAngleDt < -0.01) {
        EnemyAngle += 0.01;
    }
    EnemyAngleDt = EnemyAngle;

    float cosTheta = cos(EnemyAngle);
    float sinTheta = sin(EnemyAngle);

    //calculate player triangle window location
    ImVec2 ep1((position.x - radius * cosTheta), position.y - radius * sinTheta);
    ImVec2 ep2(position.x - radius * (cos(EnemyAngle + (2 * M_PI / 3))), position.y - radius * sin(EnemyAngle + (2 * M_PI / 3)));
    ImVec2 ep3(position.x - radius * (cos(EnemyAngle - (2 * M_PI / 3))), position.y - radius * sin(EnemyAngle - (2 * M_PI / 3)));


    //draw player triangle

    dl->AddTriangle(ep1, ep2, ep3, ImColor(255, 0, 0, 255));
}

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