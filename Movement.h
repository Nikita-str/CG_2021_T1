#ifndef MAIN_MVMNT_H
#define MAIN_MVMNT_H

#include "General.h"

struct Movement
{
    Movement(int _x, int _y) : x(_x), y(_y) { last_time = GameTime::Now().GetTime(); }
    Movement(Point pos) : Movement(pos.x, pos.y) { }

    void UpdateLastTime() { last_time = GameTime::Now().GetTime(); }

    void Move(int dx, int dy, int speed)
    {
        double now_t = GameTime::Now().GetTime();

        if (dx || dy) {
            double dt = now_t - last_time;
            x += dt * dx * speed;
            y += dt * dy * speed;
            moved = true;
            last_moved_time = now_t;
        }

        last_time = now_t;
    }

    Point Pos() { return {(int)x, (int)y}; }

    bool Moved() 
    { 
        moved = moved && (GameTime::Now().GetSecAfter(last_moved_time) < time_for_unmoved);
        return moved; 
    }

private:
    double last_time = 0;
    float x;
    float y;

    double last_moved_time = 0;
    bool moved = false;

    static constexpr double time_for_unmoved {0.2};
};

#endif