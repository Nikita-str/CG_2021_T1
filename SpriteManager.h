#ifndef MAIN_SPMz_ENE_H
#define MAIN_SPMz_ENE_H

#include "LiveObjSprite.h"

struct SpriteManager
{
    static SpriteManager &Get() { static SpriteManager spr {}; return spr; }
    
    std::vector<LiveObjSprite> enemy_spr;

private:
    static constexpr int e_get_move_frames(int type)
    {
        if (type == 0)return 8;
        return 1;
    }
    static constexpr int e_get_attack_frames(int type)
    {
        if (type == 0)return 8;
        return 1;
    }
    SpriteManager() : enemy_spr()
    {
        for (int _type = 0; _type < 1; _type++)
            enemy_spr.emplace_back(E_LiveObjType::Enemy, _type, e_get_move_frames(_type), e_get_attack_frames(_type));
    }

};

#endif