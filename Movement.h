#ifndef MAIN_MVMNT_H
#define MAIN_MVMNT_H

#include "General.h"

struct Movement
{
    Movement(int _x, int _y) : x(_x), y(_y) { last_time = GameTime::Now().GetTime(); }
    Movement(Point pos) : Movement(pos.x, pos.y) { }
    
    void SetSize(Size obj_sz) { obj_size = obj_sz; }
    void SetCanStay(bool on_empty) { can_stay_on_empty = on_empty; }

    void UpdateLastTime() { last_time = GameTime::Now().GetTime(); }

    void Move(int dx, int dy, int speed);

    Point Pos() const { return {(int)x, (int)y}; }
    Point CenterPos() const { return {(int)(x + obj_size.w/2), (int)(y + obj_size.h/2)}; }

    void SetPos(Point pos) { x = pos.x; y = pos.y; }

    bool Moved() 
    { 
        moved = moved && (GameTime::Now().GetSecAfter(last_moved_time) < time_for_unmoved);
        return moved; 
    }

private:
    double last_time = 0;
    double x;
    double y;

    bool can_stay_on_empty = false;
    Size obj_size {0, 0};

    double last_moved_time = 0;
    bool moved = false;

    static constexpr double time_for_unmoved {0.2};
};

#endif