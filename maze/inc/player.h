#pragma once
#include "point.h"

class Map;

class Player {
public:
    Player();
    void move(char wasd, const Map& m);
    Point pos;
private:
    bool tryStep(Point next, const Map& m);
};