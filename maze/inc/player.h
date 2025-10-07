#pragma once
#include "point.h"

class Map;

class Player {
public:
    Player();
    
    // 状态管理
    void initialize(const Point& startPos);
    bool isAlive() const;
    bool hasReachedExit(const Point& exitPos) const;
    
    // 移动和交互
    bool move(char direction, const Map& map);
    void takeDamage(int damage);
    
    // 属性获取
    Point getPosition() const;
    int getHealth() const;
    int getScore() const;
    
    // 可选功能
    void heal(int amount);
    void addScore(int points);
    void setMaxHealth(int newMaxHealth);
    int getMaxHealth() const;
    void reset();
    
private:
    Point position;
    int health;
    int maxHealth;
    int score;
    
    bool tryMove(const Point& newPos, const Map& map);
};