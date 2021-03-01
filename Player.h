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
    enum class E_DieType
    {
        Kill,
        EmptyStay,
    };

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

    void KeyInc() { invent.keys++; }
    void KeyDec() { invent.keys--; }
    bool CanTakeKey() const { return invent.keys < 3; }

    bool IsPlaceForItem() const { return invent.inv_item.size() < invent.inv_size; }
    std::vector<Item> &GetInv() { return invent.inv_item; }

    int BootsLvl() const { 
        return invent.boots_lvl; 
    }

    void TakeBoots(int lvl)
    {
        move_speed += lvl * 40 - invent.boots_lvl * 40;
        invent.boots_lvl = lvl;
    }

    void CheckStayOnEmpty(Point cur_pos)
    {
        auto empty = GameMap::E_TileType::Empty;
        if ((GameMap::GetCur()->PointType(Point {.x = cur_pos.x + 12, .y = cur_pos.y + 1}) == empty) &&
            (GameMap::GetCur()->PointType(Point {.x = cur_pos.x + 19, .y = cur_pos.y + 1}) == empty)) {
            die_pos = coords;
            died = true;
            die_type = E_DieType::EmptyStay;
        }
    }

    E_Dir GetCurDir()const { return spr.GetCurDir(); }
    Point GetCenter()const { return position.CenterPos(); }

    Point GetPos()const { return coords; }
    bool GetIsDied()const { return died; }
    E_DieType GetDiedType()const { return die_type; }

    void SetPosY(int y) { coords.y = y; }

    void InventoryDraw(Image &canvas) { invent.Draw(canvas); }

    void AddSpeed(int add) { 
    move_speed += add; 
    std::cout << "player speed = " << move_speed << std::endl;
    }
private:

    struct Inventory
    {
        std::vector<Item> inv_item;
        int boots_lvl = 0;
        int armor_lvl = 0;
        int keys = 0;
        int inv_size = 3;
        Inventory() : micro_inv_canvas(W_WIDTH, TILE_SZ, 4), micro_inv("../resources/micro_inv.png"), key("../resources/key.png")
        {
            for (int i = 0; i < (W_WIDTH + TILE_SZ - 1) / TILE_SZ; i++) {
                micro_inv.Draw(micro_inv_canvas, {i * TILE_SZ, 0}, true);
            }

            auto shadow_func = [](auto x) { return (x.a == 255) ? Pixel {52,52,52,255} : x; };

            //boots +++
            Image boots_shadow {"../resources/boots_3.png"};
            boots_shadow.PixelsChange(shadow_func, false);
            boots_imgs.push_back(boots_shadow);
            for (int i = 0; i < 3; i++)boots_imgs.emplace_back("../resources/boots_" + std::to_string(i + 1) + ".png");
            //boots ---

            key_black = Image::Image(key);
            key_black.PixelsChange(shadow_func, false);
        }

        void Draw(Image &canvas)
        {
            micro_inv_canvas.FastDraw(canvas, TILE_SZ, W_HEIGHT - TILE_SZ);
            int now_x_pos = 0;
            int now_y_pos = Y + 5;

            for (int i = 0; i < 3; i++) {
                int p_draw = 10 + 40 * i;
                const Image &k_draw = (i < keys) ? key : key_black;
                k_draw.Draw(canvas, Point {p_draw, now_y_pos}, true);
                now_x_pos = p_draw + 50;
            }
            boots_imgs[boots_lvl].Draw(canvas, Point {now_x_pos, now_y_pos + 3}, true);
        }

    private:
        Image micro_inv_canvas;
        Image micro_inv;
        Image key_black;
        Image key;
        std::vector<Image> boots_imgs;

        static constexpr int Y = W_HEIGHT - TILE_SZ;
    }invent;

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

    bool died = false;
    Point die_pos;
    E_DieType die_type;

    double hp = 40;

};

#endif 
//MAIN_PLAYER_H
