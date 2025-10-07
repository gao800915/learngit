#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    // 设置随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    std::cout << "==========================================" << std::endl;
    std::cout << "          迷宫游戏 v1.0" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    try {
        // 创建游戏实例并运行
        Game game;
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "游戏运行出错: " << e.what() << std::endl;
        std::cout << "按回车键退出..." << std::endl;
        std::cin.get();
        return 1;
    }
    catch (...) {
        std::cerr << "未知错误发生!" << std::endl;
        std::cout << "按回车键退出..." << std::endl;
        std::cin.get();
        return 1;
    }
    
    return 0;
}