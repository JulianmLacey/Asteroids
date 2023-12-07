#include <iostream>
#include <ostream>
#include "testcases.hpp"
#include "player.hpp"



void Tests::runTests() {
    std::cout << "Running test_getKills" << std::endl;
    //testgetKills();
    std::cout << "Running test_getTimeAlive" << std::endl;
    this->testgetTimeAlive();
    std::cout << "Running test_addBalance" << std::endl;
    this->testaddBalance();
    this->testisInventory();
    this->testisMoving();
}

void Tests::testgetKills() {
    player* testPlayer = new player();
    testPlayer->addKill();
    std::cout << "Expected output is 1" << std::endl;
    std::cout << "Tests Returns: " << testPlayer->getKills() << std::endl;
    return;
}
void Tests::testgetTimeAlive() {
    player* testPlayer = new player();
    float t = testPlayer->getTimeAlive();
    std::cout << "Expected Outoput is 0" << std::endl;
    std::cout << "Test Output: " << t << std::endl;
    delete testPlayer;
}

void Tests::testaddBalance() {
    player testPlayer;
    testPlayer.addBalance(25);
    std::cout << "Expected Ammount 1025" << std::endl;
    std::cout << "Test Ammount: " << testPlayer.getBandCount() << std::endl;
}
void Tests::testisInventory() {
    enemy testEnemy;
    std::cout << "Expected is 5" << std::endl;
    std::cout << "Test: " << testEnemy.getTimeUntilNextShot() << std::endl;
}
void Tests::testisMoving() {
    player testPlayer;
    std::cout << " Expected is false" << std::endl;
    if (testPlayer.getIsMoving() == false) {
        std::cout << "Test: is false" << std::endl;
    } else {
        std::cout << "Test is failed " << std::endl;
    }
}


void Tests::testGetPlayer() {
    gameObject gameObj;


    // Act: Assign the player instance to the gameObject's player variable


    // Assert: Check if getPlayer() returns the correct player pointer
    assert(gameObj.getPlayer() != NULL);

    // Additional assertions or output for validation
    std::cout << "Test passed successfully!\n";
}


void Tests::testgetParent() {

}


void Tests::testgetTimeUntilNextShot() {

}


void Tests::getPosition() {

}




