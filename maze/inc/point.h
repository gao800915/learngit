#pragma once

struct Point {
    int x{}, y{};
    Point() = default;
    Point(int x, int y) : x(x), y(y) {}
    
    // basic act
    bool operator==(const Point& other) const;
    Point operator+(const Point& other) const;
};