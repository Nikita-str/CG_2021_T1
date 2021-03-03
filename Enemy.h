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
        if (type == 1)return 40;
        return 30;
    }
    static constexpr bool is_fly(int type)
    {
        if (type == 0)return true;
        return false;
    }
    static constexpr Size get_sz(int type)
    {
        if (type == 0)return Size {.w = 41, .h = 34};
        if (type == 1)return Size {.w = 28, .h = 38};
        return Size {2,2};
    }
    static constexpr Size get_mov_sz(int type)
    {
        return Size {.w = 26, .h = 26};
    }
    static constexpr int get_speed(int type)
    {
        if (type == 0)return 160;
        if (type == 1)return 80;
        return 80;
    }
    static constexpr int get_r(int type)
    {
        if (type == 0)return 5;
        if (type == 1)return 3;
        return 4;
    }

public:
    Enemy(Point center_pos, int _type);

    bool IsCollide(Point pos, int border_sz);

    void Draw(Image &canvas);
    bool WasAttacked(int dmg); // return was die?
    void Move(Point player_pos);

    int id;
    Size sz {.w = 0,.h = 0};
    int hp;
    int max_hp;
    Movement mov;
    int speed = 50;

    int type;

    E_Dir cur_dir;
    E_LiveObjState cur_state;

    double hit_take_time = -1;

    bool alive = true;
};
#endif