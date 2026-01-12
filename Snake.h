#ifndef SNAKE_H
#define SNAKE_H

#include "Common.h"
#include <deque>

class Snake {
private:
    std::deque<Point> body;
    Direction dir;
    bool alive;
    int score;
public:
    Snake(int startX, int startY);
    void setDirection(Direction d);
    Point move(bool grow);
    bool checkSelfCollision() const;
    void teleport(Point p); // 传送逻辑
    
    Point getHead() const { return body.front(); }
    const std::deque<Point>& getBody() const { return body; }
    bool isAlive() const { return alive; }
    void die() { alive = false; }
    void addScore(int val) { score += val; }
    int getScore() const { return score; }
};

#endif