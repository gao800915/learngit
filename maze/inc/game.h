#pragma once
#include "map.h"
#include "player.h"

class Game {
public:
    Game();
    ~Game() = default;
    
    // 游戏流程控制
    void run();
    
private:
    Map currentMap;
    Player player;
    bool gameRunning;
    
    // 游戏状态
    void mainMenu();
    void selectMap();
    void gameLoop();
    void displayGameState() const;
    
    // 用户输入
    char getPlayerInput() const;
    void handleInput(char input);
    
    // 游戏逻辑
    void checkTraps();
    void checkWinCondition(bool& levelCompleted); // 添加参数
};