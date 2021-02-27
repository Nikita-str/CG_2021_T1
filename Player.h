#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "LiveObjSprite.h"
#include "General.h"
#include "Movement.h"

struct Player
{
    //explicit Player(Point pos = {.x = 10, .y = 10}) : coords(pos), old_coords(coords) {};
private: static Player *player_getter;
public:
    static Player &Get() { return *player_getter; }

    explicit Player(Point pos, LiveObjSprite &sprite) : position(pos), coords(pos), old_coords(pos), spr(sprite) 
    {
        position.SetSize(Size {.w = 29, .h = 29}); player_getter = this;
    };

    void Attack();

    bool Moved();
    void Draw(Image &screen);

    inline void RefreshMoveState(bool up, bool down, bool left, bool right)
    {
        RefreshMoveState((left == right) ? E_X_Dir::Not : (left ? E_X_Dir::Left : E_X_Dir::Right),
                         (up == down) ? E_Y_Dir::Not : (up ? E_Y_Dir::Up : E_Y_Dir::Down));
    }
    void RefreshMoveState(E_X_Dir x, E_Y_Dir y);

    void KeyInc() { keys++; }
    void KeyDec() { keys--; }
private:
    E_Dir back_dir = E_Dir::DOWN;

    Movement position;
    Point coords {.x = 0, .y = 0};
    Point old_coords {.x = 0, .y = 0};

    Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed = 100;

    LiveObjSprite &spr;

    bool now_attack = false;
    double blocked_to_time = -0.0;

    int keys = 0;
};

#endif //MAIN_PLAYER_H
