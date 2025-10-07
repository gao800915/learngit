#pragma once
#include "point.h"
#include <vector>
#include <string>

enum class CellType {
    EMPTY,      // 可走路径
    WALL,       // 墙壁
    TRAP,       // 陷阱
    START,      // 起点
    EXIT        // 终点
};

class Map {
public:
    Map();
    ~Map() = default;
    
    // 地图管理
    bool loadFromFile(const std::string& filename);
    void createRandomMap(int width, int height);
    
    // 地图信息
    int getWidth() const;
    int getHeight() const;
    CellType getCellType(const Point& pos) const;
    Point getStartPosition() const;
    Point getExitPosition() const;
    // 碰撞检测
    bool isWalkable(const Point& pos) const;
    bool isTrap(const Point& pos) const;
    void removeTrap(const Point& pos);
    
    // 显示
    void display() const;
    void displayWithPlayer(const Point& playerPos) const;
    
private:
    std::vector<std::vector<CellType>> grid;
    Point startPos;
    Point exitPos;
    std::vector<Point> traps;
    
    void initializeGrid(int width, int height);
};