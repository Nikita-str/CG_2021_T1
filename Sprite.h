#ifndef MAIN_SPRITE_H
#define MAIN_SPRITE_H
#include <string>
#include <vector>

#include "General.h"
#include "Image.h"

struct SpritePixSz
{
    int width;
    SpritePixSz(int w) : width(w) {}
};

struct Sprite
{
    Sprite(const std::string &path, int p_frames, int ms_on_frame = 125, int scale = 1);
    Sprite(const std::string &path, SpritePixSz frame_sz, int ms_on_frame = 125, int scale = 1);

    void Draw(Image &canvas, const Point p, bool flip_OX = false, bool flip_OY = false);

private:
    void SpriteFromImg(Image &img, int p_frames, int ms_on_frame = 125, int scale = 1);

    int frames = 0;
    int frame_now = 0;
    double s_per_frame = 0.075;

    double time_start_prev_frame = 0;
    std::vector<Image> imgs;
};

#endif