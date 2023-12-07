#include "player.hpp"
class Tests {

    public:
    void runTests(){
        testgetKills();
        testgetTimeAlive();
        testaddBalance();
        testisInventory();
        testisMoving();
    }

    private:

    void testgetKills()
    {
        player *testPlayer;
        testPlayer->addKill();

        cout << "Tests Kills: " <<testPlayer->getKills() << std::endl;
        cout <<"Expected output is 1" << std::endl;
    }

    void testgetTimeAlive()
    {
        player *testPlayer;
        
        cout << "Expected Outoput is 0" << std::endl;

        cout << "Test Output: " << testPlayer->getTimeAlive() << std::endl;
    }

    void testaddBalance()
    {
        player *testPlayer;

        testPlayer->addBalance(25);

        cout << "Expected Ammount 25" << std::endl;

        cout << "Test Ammount: " << testPlayer->getBandCount() << std::endl;

    }

    void testisInventory()
    {
        enemy *testEnemy;

        

        cout << "Expected is 5" << std::endl;

        cout << "Test: " << testEnemy->getTimeUntilNextShot() << std::endl;

        
    }

    void testisMoving()
    {
        player *testPlayer;

        cout << " Expected is false" <<std::endl;

        if(testPlayer->getIsMoving() == false)
        {
            cout << "Test: is false" <<std::endl;
        }
        else{
            cout << "Test is failed " <<std::endl;
        }
    }
};