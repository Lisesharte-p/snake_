#ifndef ENTITY_H
#define ENTITY_H

#include "Common.h"

// 抽象道具基类
class GameItem {
protected:
    Point pos;
    char icon;
public:
    GameItem(char i) : icon(i), pos({0, 0}) {}
    virtual ~GameItem() {}
    void setPos(Point p) { pos = p; }
    Point getPos() const { return pos; }
    char getIcon() const { return icon; }

    virtual void onInteract(class Snake& s) = 0; 
};

// 食物
class Food : public GameItem {
public:
    Food() : GameItem('*') {}
    void onInteract(class Snake& s) override; 
};

// 障碍物 
class Obstacle : public GameItem {
public:
    Obstacle() : GameItem('#') {}
    void onInteract(class Snake& s) override;
};

// 传送门
class Portal : public GameItem {
public:
    Portal() : GameItem('@') {}
    void onInteract(class Snake& s) override;
};

#endif