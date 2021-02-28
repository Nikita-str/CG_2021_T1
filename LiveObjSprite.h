#ifndef MAIN_LO_SPRITE_H
#define MAIN_LO_SPRITE_H

#include "Sprite.h"
#include <map>

enum class E_LiveObjState
{
    Idle,
    Walk,
    Attack
};

struct LiveObjSprite
{

    LiveObjSprite(const std::string &path0, SpritePixSz spr_sz, int ms_on_frame = 125, int scale = 1) : spr_states()
    {
        E_LiveObjState lo_states[] {E_LiveObjState::Idle, E_LiveObjState::Walk, E_LiveObjState::Attack};
        std::string paths[] {path0 + "Idle/", path0 + "Walk/", path0 + "Attack/"};
        std::map<E_Dir, std::string> dirs {{E_Dir::UP, "up.png"}, {E_Dir::DOWN, "down.png"}, {E_Dir::LEFT, "left.png"}, {E_Dir::RIGHT, "right.png"}};

        
        for(int i = 0; i < sizeof(lo_states) / sizeof(E_LiveObjState); i++)
        {
            auto state = lo_states[i];
            auto path = paths[i];
            auto &now_state = spr_states.insert({state, std::map<E_Dir, Sprite>{}}).first->second;

            for (auto iter = dirs.begin(); iter != dirs.end(); ++iter) {
                auto img_path = path + iter->second;
                now_state.insert({iter->first, Sprite(img_path, spr_sz, ms_on_frame, scale)});
            }
        }

        SetCurSprite();
    }

    bool SetDir(E_Dir new_dir)
    {
        if (cur_dir != new_dir) {
            cur_dir = new_dir;
            SpritePrepare();
            return true;
        }
        return false;
    }

    bool SetState(E_LiveObjState new_state)
    {
        if (cur_state != new_state) {
            cur_state = new_state;
            SpritePrepare();
            return true;
        }
        return false;
    }

    double GetCurAnimTime() const { return cur_spr->AnimTime(); }

    void Draw(Image &canvas, const Point p, bool flip_OX = true, bool flip_OY = false)
    {
        cur_spr->Draw(canvas, p, flip_OX, flip_OY);
    }

    const Image& GetImage(E_LiveObjState state, E_Dir dir, int frame) const {return spr_states.find(state)->second.find(dir)->second.GetFrame(frame);}
    const Image& GetImage(E_LiveObjState state, int frame) const {return spr_states.find(state)->second.find(cur_dir)->second.GetFrame(frame);}

private:
    void SpritePrepare()
    {
        SetCurSprite();
        cur_spr->Restart();
    }

    inline void SetCurSprite()
    {
        cur_spr = &spr_states.find(cur_state)->second.find(cur_dir)->second;
        if (cur_spr == nullptr) {
            cur_spr = &spr_states.find(E_LiveObjState::Idle)->second.find(cur_dir)->second;
        }
    }

    std::map< E_LiveObjState, std::map<E_Dir, Sprite>> spr_states;
    E_Dir cur_dir = E_Dir::DOWN;
    E_LiveObjState cur_state = E_LiveObjState::Idle;

    Sprite *cur_spr = nullptr;
};

#endif