#ifndef MAIN_IT_EM_H
#define MAIN_IT_EM_H

#include "Sprite.h"

typedef std::function<void(bool)> item_action; // bool : true => player take it;  false => player throw it;
typedef std::function<bool(void)> item_can_take;
typedef std::function<void(void)> item_use;

enum class E_ItemTypes
{
    Nothing,
    Key,
    Boots,
    Ring,
    Potion,
    Book,
    Other
};

struct Item
{
    E_ItemTypes item_type = E_ItemTypes::Nothing;
    int item_lvl = -2;
    Point pos = Point{0, 0};
    Sprite spr;
    item_action ia;
    item_can_take ict;
    item_use use;
    bool can_be_used = false;
    bool inventory = false;
    Item() {}
    //Item(Item && itm) : item_type(itm.item_type), item_lvl(itm.item_lvl), pos(itm.pos), spr(itm.spr), ia(itm.ia), ict(itm.ict), inventory(itm.inventory){}
    //Item(E_ItemTypes i_type, Point _pos, Sprite _spr, item_can_take _ict, item_action _ia) : item_type(i_type), spr(_spr), pos(_pos), ia(_ia), ict(_ict) {}
    Item(E_ItemTypes i_type, int i_lvl, Point _pos);
    Item(int i_lvl, Point _pos);

    void empty_ia(bool on) {}

    bool IsPointIn(Point p)
    {
        int w = spr.Width();
        int h = spr.Height();
        int p_x = pos.x * TILE_SZ + (TILE_SZ - w) / 2;
        int p_y = pos.y * TILE_SZ + (TILE_SZ - h) / 2;
        if (p.x < p_x)return false;
        if (p.y < p_y)return false;
        if (p.x > p_x + w)return false;
        if (p.y > p_y + h)return false;
        return true; 
    }
    bool IsRectIn(Point p, Size sz)
    {
        int w = spr.Width();
        int h = spr.Height();
        int p_x = pos.x * TILE_SZ + (TILE_SZ - w) / 2;
        int p_y = pos.y * TILE_SZ + (TILE_SZ - h) / 2;
        if (p.x < p_x && p.x + sz.w < p_x)return false;
        if (p.x > p_x && p.x > p_x + w)return false;
        if (p.y < p_y && p.y + sz.h < p_y)return false;
        if (p.y > p_y && p.y > p_y + h)return false;
        return true;
    }
private:
    void CreateHelper();
};

#endif

