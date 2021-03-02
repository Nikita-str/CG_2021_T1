#ifndef MAIN_I3z_ENE_H
#define MAIN_I3z_ENE_H

#include "SpriteManager.h"
#include "Movement.h"

struct Enemy
{
private:
    static constexpr int get_hp(int type)
    {
        if (type == 0)return 20;
        return 35;
    }
    static constexpr Size get_sz(int type)
    {
        if (type == 0)return Size {.w = 41, .h = 34};
        return Size {2,2};
    }
    static constexpr int get_speed(int type)
    {
        if (type == 0)return 160;
        return 80;
    }
public:
    Enemy(Point center_pos, int _type);

    void Draw(Image &canvas);
    
    Size sz {.w = 0,.h = 0};
    int hp;
    Movement mov;
    int speed = 50;

    int type;

    E_Dir cur_dir;
};
#endif