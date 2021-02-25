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
};

#endif

