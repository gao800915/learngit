#include "player.h"
#include "map.h"
#include <iostream>

// 构造函数
Player::Player() : health(3), maxHealth(3), score(0) {
    // 位置将在initialize中设置
}

// 初始化玩家
void Player::initialize(const Point& startPos) {
    position = startPos;
    health = maxHealth;
    score = 0;
    std::cout << "玩家初始化完成，生命值: " << health << std::endl;
}

// 检查玩家是否存活
bool Player::isAlive() const {
    return health > 0;
}

// 检查是否到达出口
bool Player::hasReachedExit(const Point& exitPos) const {
    return position.x == exitPos.x && position.y == exitPos.y;
}

// 移动玩家
bool Player::move(char direction, const Map& map) {
    Point newPos = position;
    
    // 根据方向计算新位置
    switch (direction) {
        case 'w': // 上
        case 'W':
            newPos.y--;
            break;
        case 's': // 下
        case 'S':
            newPos.y++;
            break;
        case 'a': // 左
        case 'A':
            newPos.x--;
            break;
        case 'd': // 右
        case 'D':
            newPos.x++;
            break;
        default:
            std::cout << "无效的移动方向!" << std::endl;
            return false;
    }
    
    // 尝试移动到新位置
    if (tryMove(newPos, map)) {
        position = newPos;
        
        // 显示移动信息
        std::string dirStr;
        switch (direction) {
            case 'w': case 'W': dirStr = "上"; break;
            case 's': case 'S': dirStr = "下"; break;
            case 'a': case 'A': dirStr = "左"; break;
            case 'd': case 'D': dirStr = "右"; break;
        }
        std::cout << "向" << dirStr << "移动成功" << std::endl;
        
        return true;
    } else {
        std::cout << "无法向该方向移动！" << std::endl;
        return false;
    }
}

// 玩家受伤
void Player::takeDamage(int damage) {
    if (damage > 0) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
        std::cout << "受到 " << damage << " 点伤害！" << std::endl;
    }
}

// 获取玩家位置
Point Player::getPosition() const {
    return position;
}

// 获取生命值
int Player::getHealth() const {
    return health;
}

// 获取分数
int Player::getScore() const {
    return score;
}

// 尝试移动
bool Player::tryMove(const Point& newPos, const Map& map) {
    // 检查新位置是否可走
    if (!map.isWalkable(newPos)) {
        return false;
    }
    
    return true;
}

// 增加生命值（可选功能）
void Player::heal(int amount) {
    if (amount > 0) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
        std::cout << "恢复 " << amount << " 点生命值！当前生命值: " << health << std::endl;
    }
}

// 增加分数（可选功能）
void Player::addScore(int points) {
    if (points > 0) {
        score += points;
        std::cout << "获得 " << points << " 分！当前分数: " << score << std::endl;
    }
}

// 设置最大生命值（可选功能）
void Player::setMaxHealth(int newMaxHealth) {
    if (newMaxHealth > 0) {
        maxHealth = newMaxHealth;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }
}

// 获取最大生命值（可选功能）
int Player::getMaxHealth() const {
    return maxHealth;
}

// 重置玩家状态（可选功能）
void Player::reset() {
    health = maxHealth;
    score = 0;
    // 位置保持不变，通常会在initialize中重新设置
}