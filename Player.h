#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "LiveObjSprite.h"
#include "General.h"

struct Player
{
    //explicit Player(Point pos = {.x = 10, .y = 10}) : coords(pos), old_coords(coords) {};

    explicit Player(Point pos, LiveObjSprite &sprite) : coords(pos), old_coords(coords), spr(sprite) {};

    bool Moved() const;
    void Draw(Image &screen);

    inline void RefreshMoveState(bool up, bool down, bool left, bool right)
    {
        RefreshMoveState((left == right) ? E_X_Dir::Not : (left ? E_X_Dir::Left : E_X_Dir::Right),
                         (up == down) ? E_Y_Dir::Not : (up ? E_Y_Dir::Up : E_Y_Dir::Down));
    }
    void RefreshMoveState(E_X_Dir x, E_Y_Dir y);
private:
    void RefreshMoveState(E_Dir dir);
    E_Dir back_dir = E_Dir::DOWN;

    Point coords {.x = 10, .y = 10};
    Point old_coords {.x = 10, .y = 10};
    Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed = 4;

    LiveObjSprite &spr;

};

#endif //MAIN_PLAYER_H
