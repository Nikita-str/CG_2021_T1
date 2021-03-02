#ifndef MAIN_SPMz_ENE_H
#define MAIN_SPMz_ENE_H

#include "LiveObjSprite.h"

struct SpriteManager
{
    static SpriteManager &Get() { static SpriteManager spr {}; return spr; }
    
    std::vector<LiveObjSprite> enemy_spr;
    std::vector<Image> hp_imgs;

    void DrawHpBlock(Image &canvas, Point pos, int hp)
    {
        if (hp >= 10) {
            hp_imgs[10].Draw(canvas, pos);
        } else if (hp < 0) {
            hp_imgs[0].Draw(canvas, pos);
        } else {
            hp_imgs[hp].Draw(canvas, pos);
        }
    }

    static constexpr int HP_SZ = 6;

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
    static constexpr int e_get_take_hit_frames(int type)
    {
        if (type == 0)return 4;
        return 1;
    }
    SpriteManager() : enemy_spr(), hp_imgs()
    {
        for (int _type = 0; _type < 1; _type++)
            enemy_spr.emplace_back(E_LiveObjType::Enemy, _type, e_get_move_frames(_type), e_get_attack_frames(_type), e_get_take_hit_frames(_type));

        int sz = HP_SZ;
        Image hp_img {sz,sz,4};
        hp_img.PixelsChange([](auto x) {return Pixel {52, 52, 52, 255}; }, false);
        hp_imgs.push_back(std::move(hp_img));
        for (int i = 1; i <= 10; i++) {
            Image temp_img {sz,sz,4};
            temp_img.PixelsChange([z = i](auto x) {return Pixel {(uint8_t)(52 + z * 2), (uint8_t)(52 + z * 11), (uint8_t)(52 - z * 5), 255}; }, false);
            hp_imgs.push_back(std::move(temp_img));
        }
    }

};

#endif