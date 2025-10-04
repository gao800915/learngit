
#pragma once
#include "map.h"
#include "player.h"

class Game {
public:
    Game();
    ~Game() = default;
    
    // control the process of game
    void run();
    
private:
    Map currentMap;
    Player player;
    bool gameRunning;
    
    // the status of game
    void mainMenu();
    void selectMap();
    void gameLoop();
    void displayGameState() const;
    
    // user input
    char getPlayerInput() const;
    void handleInput(char input);
    
    // logic of the game
    void checkTraps();
    void checkWinCondition();
};