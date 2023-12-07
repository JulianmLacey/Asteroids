#pragma once
#include <iostream>

using namespace std;

class Tests {
public:
    Tests() {};
    ~Tests() {};

    void runTests();
private:
    void testgetKills();
    void testgetTimeAlive();
    void testaddBalance();
    void testisInventory();
    void testisMoving();
    void testGetPlayer();
    void testgetParent();
    void testgetTimeUntilNextShot();
    void getPosition();
};
