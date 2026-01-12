#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <windows.h>
#include <vector>


struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};


enum Direction { UP, DOWN, LEFT, RIGHT };


enum GameMode { SIMPLE = 1, OBSTACLE = 2, PORTAL = 3 };


class Console {
public:
    static void setPos(int x, int y) {
        COORD pos = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }
    static void hideCursor() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hOut, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hOut, &cursorInfo);
    }
};

#endif