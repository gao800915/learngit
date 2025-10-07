#include "game.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// construct function
Game::Game() : gameRunning(false) {
    // initialize the status of game
}

// main run function
void Game::run() {
    std::cout << "=== maze game===" << std::endl;
    std::cout << "press enter to start..." << std::endl;
    std::cin.get();
    
    gameRunning = true;
    
    while (gameRunning) {
        mainMenu();
        if (gameRunning) {
            selectMap();
            if (gameRunning) {
                gameLoop();
            }
        }
    }
    
    std::cout << "thank you for palying my game" << std::endl;
}

// 主菜单
void Game::mainMenu() {
    int choice = 0;
    
    while (choice != 1 && choice != 2) {
        std::cout << "\n========== main menu==========" << std::endl;
        std::cout << "1. start the game" << std::endl;
        std::cout << "2. exit the game" << std::endl;
        std::cout << "please choose the game (1-2): ";
        
        std::cin >> choice;
        std::cin.ignore(); // clear the buffer
        
        switch (choice) {
            case 1:
                std::cout << "start the game..." << std::endl;
                break;
            case 2:
                gameRunning = false;
                std::cout << "exit the game..." << std::endl;
                break;
            default:
                std::cout << "your choise is wrong" << std::endl;
                break;
        }
    }
}

// select the map
void Game::selectMap() {
    int choice = 0;
    std::vector<std::string> mapFiles = {
        "maps/map1.txt",
        "maps/map2.txt"
    };
    
    while (choice < 1 || choice > mapFiles.size()) {
        std::cout << "\n========== choose your map =========" << std::endl;
        std::cout << "1. easy" << std::endl;
        std::cout << "2. difficult" << std::endl;
        std::cout <<"please choose your map (1-" << mapFiles.size() << "): ";
        
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice >= 1 && choice <= mapFiles.size()) {
            if (currentMap.loadFromFile(mapFiles[choice - 1])) {
                std::cout << "the map is ready" << std::endl;
                player.initialize(currentMap.getStartPosition());
            } else {
                std::cout << "the map fail to create..." << std::endl;
                currentMap.createRandomMap(15, 10);
                player.initialize(currentMap.getStartPosition());
            }
        } else {
            std::cout << "wrong choise" << std::endl;
            choice = 0;
        }
    }
}

// main loop
void Game::gameLoop() {
    std::cout << "\n========== start the game ==========" << std::endl;
    std::cout << "use WASD to move,Q exit the game" << std::endl;
    std::cout << "caution the trap" << std::endl;
    
    bool levelCompleted = false;
    
    while (!levelCompleted && player.isAlive() && gameRunning) {
        // display the status of game
        displayGameState();
        
        // get the input of user
        char input = getPlayerInput();
        
        //process the command of exit
        if (input == 'q' || input == 'Q') {
            std::cout << "返回主菜单..." << std::endl;
            break;
        }
        
        // handle move
        handleInput(input);
        
        // check the trap
        checkTraps();
        
        //
        checkWinCondition(levelCompleted);
        
        // check whether user died
        if (!player.isAlive()) {
            std::cout << "游戏结束！你死了！" << std::endl;
            std::cout << "按回车键继续..." << std::endl;
            std::cin.get();
        }
        
        // check whether finished
        if (levelCompleted) {
            std::cout << "恭喜！你找到了出口！" << std::endl;
            std::cout << "按回车键继续..." << std::endl;
            std::cin.get();
        }
    }
}


void Game::displayGameState() const {
    std::cout << "\n========== 游戏状态 ==========" << std::endl;
    std::cout << "生命值: " << player.getHealth() << std::endl;
    std::cout << "当前位置: (" << player.getPosition().x << ", " << player.getPosition().y << ")" << std::endl;
    
    // dispaly map and player
    currentMap.displayWithPlayer(player.getPosition());
    
    std::cout << "==============================" << std::endl;
}

//
char Game::getPlayerInput() const {
    char input = '\0';
    
    while (true) {
        std::cout << "input move direction(WASD) or Q exit: ";
        std::string inputStr;
        std::getline(std::cin, inputStr);
        
        if (!inputStr.empty()) {
            input = std::tolower(inputStr[0]);
            
            // check whether input efficient
            if (input == 'w' || input == 'a' || input == 's' || input == 'd' || input == 'q') {
                break;
            }
        }
        
        std::cout << "inefficient input W(up), A(left), S(down), D(right) 或 Q(exit)" << std::endl;
    }
    
    return input;
}

// 
void Game::handleInput(char input) {
    if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
        bool moved = player.move(input, currentMap);
        if (moved) {
            std::cout << "move successfully" << std::endl;
        } else {
            std::cout << "cant move in this direction" << std::endl;
        }
    }
}

// 
void Game::checkTraps() {
    Point playerPos = player.getPosition();
    
    if (currentMap.isTrap(playerPos)) {
        std::cout << "ops,you are trapped" << std::endl;
        player.takeDamage(1); // HP declines 1
        currentMap.removeTrap(playerPos);
        std::cout << "your current HP: " << player.getHealth() << std::endl;
    }
}

// 
// 其他函数保持不变...

// 检查胜利条件
void Game::checkWinCondition(bool& levelCompleted) {
    Point playerPos = player.getPosition();
    Point exitPos = currentMap.getExitPosition();
    
    if (playerPos.x == exitPos.x && playerPos.y == exitPos.y) {
        levelCompleted = true;
    }
}