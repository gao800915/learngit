#include "map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <stdexcept>

// 构造函数
Map::Map() {
    // 初始化一个默认的空地图
    initializeGrid(15, 10);
}

// 从文件加载地图
bool Map::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开地图文件: " << filename << std::endl;
        return false;
    }
    
    try {
        int width, height;
        file >> width >> height;
        
        // 检查地图尺寸是否符合要求
        if (width < 15 || height < 10) {
            std::cerr << "地图尺寸太小，至少需要15x10" << std::endl;
            return false;
        }
        
        // 初始化网格
        initializeGrid(width, height);
        
        // 读取地图数据
        std::string line;
        std::getline(file, line); // 读取第一行后的换行符
        
        traps.clear(); // 清空陷阱列表
        
        for (int y = 0; y < height; ++y) {
            std::getline(file, line);
            for (int x = 0; x < width && x < static_cast<int>(line.length()); ++x) {
                char cell = line[x];
                Point pos(x, y);
                
                switch (cell) {
                    case '#': // 墙壁
                        grid[y][x] = CellType::WALL;
                        break;
                    case ' ': // 空地
                    case '.':
                        grid[y][x] = CellType::EMPTY;
                        break;
                    case 'S': // 起点
                        grid[y][x] = CellType::START;
                        startPos = pos;
                        break;
                    case 'E': // 终点
                        grid[y][x] = CellType::EXIT;
                        exitPos = pos;
                        break;
                    case 'T': // 陷阱
                        grid[y][x] = CellType::TRAP;
                        traps.push_back(pos);
                        break;
                    default:
                        grid[y][x] = CellType::EMPTY;
                        break;
                }
            }
        }
        
        file.close();
        std::cout << "地图加载成功: " << filename << " (" << width << "x" << height << ")" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "读取地图文件时出错: " << e.what() << std::endl;
        return false;
    }
}

// 创建随机地图
void Map::createRandomMap(int width, int height) {
    // 设置最小尺寸
    if (width < 15) width = 15;
    if (height < 10) height = 10;
    
    initializeGrid(width, height);
    
    // 随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);
    
    // 生成随机墙壁
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // 边界设置为墙壁
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                grid[y][x] = CellType::WALL;
            } else {
                // 内部区域随机生成墙壁
                if (dis(gen) < 30) { // 30%的概率生成墙壁
                    grid[y][x] = CellType::WALL;
                } else {
                    grid[y][x] = CellType::EMPTY;
                }
            }
        }
    }
    
    // 设置起点和终点
    startPos = Point(1, 1);
    exitPos = Point(width - 2, height - 2);
    grid[startPos.y][startPos.x] = CellType::START;
    grid[exitPos.y][exitPos.x] = CellType::EXIT;
    
    // 生成随机陷阱
    traps.clear();
    int trapCount = (width * height) / 20; // 大约5%的格子是陷阱
    
    for (int i = 0; i < trapCount; ++i) {
        int x, y;
        do {
            x = std::uniform_int_distribution<>(1, width - 2)(gen);
            y = std::uniform_int_distribution<>(1, height - 2)(gen);
        } while (grid[y][x] != CellType::EMPTY || 
                 (x == startPos.x && y == startPos.y) || 
                 (x == exitPos.x && y == exitPos.y));
        
        grid[y][x] = CellType::TRAP;
        traps.push_back(Point(x, y));
    }
    
    // 确保起点到终点有路径（简单方法：清除一条路径）
    // 从起点向右和向下清除一条路径
    for (int x = startPos.x; x <= exitPos.x; ++x) {
        if (grid[startPos.y][x] == CellType::WALL) {
            grid[startPos.y][x] = CellType::EMPTY;
        }
    }
    
    for (int y = startPos.y; y <= exitPos.y; ++y) {
        if (grid[y][exitPos.x] == CellType::WALL) {
            grid[y][exitPos.x] = CellType::EMPTY;
        }
    }
    
    std::cout << "随机地图生成成功: " << width << "x" << height << std::endl;
}

// 获取地图宽度
int Map::getWidth() const {
    if (grid.empty()) return 0;
    return grid[0].size();
}

// 获取地图高度
int Map::getHeight() const {
    return grid.size();
}

// 获取单元格类型
CellType Map::getCellType(const Point& pos) const {
    if (pos.x < 0 || pos.x >= getWidth() || pos.y < 0 || pos.y >= getHeight()) {
        return CellType::WALL; // 超出边界视为墙壁
    }
    return grid[pos.y][pos.x];
}

// 获取起点位置
Point Map::getStartPosition() const {
    return startPos;
}

// 检查位置是否可走
bool Map::isWalkable(const Point& pos) const {
    CellType type = getCellType(pos);
    return type == CellType::EMPTY || 
           type == CellType::START || 
           type == CellType::EXIT || 
           type == CellType::TRAP;
}

// 检查位置是否是陷阱
bool Map::isTrap(const Point& pos) const {
    return getCellType(pos) == CellType::TRAP;
}

// 移除陷阱
void Map::removeTrap(const Point& pos) {
    if (pos.x < 0 || pos.x >= getWidth() || pos.y < 0 || pos.y >= getHeight()) {
        return;
    }
    
    if (grid[pos.y][pos.x] == CellType::TRAP) {
        grid[pos.y][pos.x] = CellType::EMPTY;
        
        // 从陷阱列表中移除
        for (auto it = traps.begin(); it != traps.end(); ++it) {
            if (it->x == pos.x && it->y == pos.y) {
                traps.erase(it);
                break;
            }
        }
    }
}

// 显示地图
void Map::display() const {
    for (int y = 0; y < getHeight(); ++y) {
        for (int x = 0; x < getWidth(); ++x) {
            Point pos(x, y);
            char symbol = ' ';
            
            switch (getCellType(pos)) {
                case CellType::WALL:
                    symbol = '#'; // 墙壁
                    break;
                case CellType::EMPTY:
                    symbol = ' '; // 空地
                    break;
                case CellType::TRAP:
                    symbol = 'T'; // 陷阱
                    break;
                case CellType::START:
                    symbol = 'S'; // 起点
                    break;
                case CellType::EXIT:
                    symbol = 'E'; // 终点
                    break;
            }
            
            std::cout << symbol;
        }
        std::cout << std::endl;
    }
}

// 显示带玩家的地图
void Map::displayWithPlayer(const Point& playerPos) const {
    for (int y = 0; y < getHeight(); ++y) {
        for (int x = 0; x < getWidth(); ++x) {
            Point pos(x, y);
            
            // 如果这是玩家位置，显示玩家
            if (pos.x == playerPos.x && pos.y == playerPos.y) {
                std::cout << 'P'; // 玩家
                continue;
            }
            
            char symbol = ' ';
            
            switch (getCellType(pos)) {
                case CellType::WALL:
                    symbol = '#'; // 墙壁
                    break;
                case CellType::EMPTY:
                    symbol = ' '; // 空地
                    break;
                case CellType::TRAP:
                    symbol = 'T'; // 陷阱
                    break;
                case CellType::START:
                    symbol = 'S'; // 起点
                    break;
                case CellType::EXIT:
                    symbol = 'E'; // 终点
                    break;
            }
            
            std::cout << symbol;
        }
        std::cout << std::endl;
    }
}

// 初始化网格
void Map::initializeGrid(int width, int height) {
    grid.resize(height);
    for (int y = 0; y < height; ++y) {
        grid[y].resize(width, CellType::EMPTY);
    }
    
    // 设置边界为墙壁
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                grid[y][x] = CellType::WALL;
            }
        }
    }
    
    // 设置默认起点和终点
    startPos = Point(1, 1);
    exitPos = Point(width - 2, height - 2);
    grid[startPos.y][startPos.x] = CellType::START;
    grid[exitPos.y][exitPos.x] = CellType::EXIT;
    
    // 清空陷阱列表
    traps.clear();
}
Point Map::getExitPosition() const {
    return exitPos;
}