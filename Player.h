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
        position.SetSize(Size {.w = 29, .h = 29});
        position.SetCanStay(true);
        player_getter = this;
    };

    void Attack();

    bool Moved();
    void Draw(Image &screen);
    void DieDraw(Image &canvas, double proc);

    inline void RefreshMoveState(bool up, bool down, bool left, bool right, bool ctrl)
    {
        RefreshMoveState((left == right) ? E_X_Dir::Not : (left ? E_X_Dir::Left : E_X_Dir::Right),
                         (up == down) ? E_Y_Dir::Not : (up ? E_Y_Dir::Up : E_Y_Dir::Down), ctrl);
    }
    void RefreshMoveState(E_X_Dir x, E_Y_Dir y, bool ctrl);

    void KeyInc() { keys++; }
    void KeyDec() { keys--; }

    void CheckStayOnEmpty(Point cur_pos)
    {
        auto empty = GameMap::E_TileType::Empty;
        if ((GameMap::GetCur()->PointType(Point {.x = cur_pos.x + 12, .y = cur_pos.y + 1}) == empty) &&
            (GameMap::GetCur()->PointType(Point {.x = cur_pos.x + 19, .y = cur_pos.y + 1}) == empty)) {
            die_pos = coords;
            died = true;
        }
    }

    Point GetPos()const { return coords; }
    bool GetIsDied()const { return died; }

    void SetPosY(int y) { coords.y = y; }
private:
    E_Dir back_dir = E_Dir::DOWN;

    Movement position;
    Point coords {.x = 0, .y = 0};
    Point old_coords {.x = 0, .y = 0};

    Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
    int move_speed = 100;
    int move_speed_ctrl = 40;

    LiveObjSprite &spr;

    bool now_attack = false;
    double blocked_to_time = -0.0;

    int keys = 0;

    bool died = false;
    Point die_pos;
};

#endif //MAIN_PLAYER_H
