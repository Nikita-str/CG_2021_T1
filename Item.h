#ifndef MAIN_IT_EM_H
#define MAIN_IT_EM_H

#include "Sprite.h"

typedef std::function<void(bool)> item_action; // bool : true => player take it;  false => player throw it;

struct Item
{
    Point pos;
    Sprite spr;
    item_action ia;
    Item(Point _pos, Sprite _spr, item_action _ia) : spr(_spr), pos(_pos), ia(_ia) {}

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
};

#endif

