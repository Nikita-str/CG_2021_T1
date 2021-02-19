#include "Sprite.h"
#include <iostream>
#include <utility>

void Sprite::SpriteFromImg(Image &img0, int p_frames, int ms_on_frame, int scale)
{
    frames  = p_frames;
    s_per_frame = ms_on_frame / 1000.0;

    if (p_frames <= 0) {
        std::cerr << "too few amount of frames : " << p_frames << std::endl;
        std::abort();
    }

    if (frames == 1) {
        imgs.emplace_back(Image {img0});
        return;
    }

    int w = img0.Width();
    int h = img0.Height();
    int chans = img0.Channels();

    int sz_w = w / frames;
    int sz_h = h;

    int temp_x_now = 0;

    for (int frame = 0; frame < frames; frame++, temp_x_now -= sz_w) {
        imgs.emplace_back(Image {sz_w, sz_h, chans});
        img0.Draw(imgs[frame], {.x = temp_x_now, .y = 0}, false, false, true);
    }
}

Sprite::Sprite(const std::string &path, int p_frames, int ms_on_frame, int scale) : imgs()
{
    SpriteFromImg(Image {path, scale}, p_frames, ms_on_frame, scale);
}

Sprite::Sprite(const std::string &path, SpritePixSz frame_sz, int ms_on_frame, int scale) : imgs()
{
    auto img0 = Image {path, scale};
    if (frame_sz.width == 0)std::abort();
    int p_frames = img0.Width() / (frame_sz.width * scale);
    SpriteFromImg(img0, p_frames, ms_on_frame, scale);
}


void Sprite::Draw(Image &canvas, const Point p, bool flip_OX, bool flip_OY)
{
    double dt = GameTime::Now().GetSecAfter(time_start_prev_frame);
    if (dt < 0)dt = 0;
    
    int add_frame = 0;

    if (dt > s_per_frame) {
        add_frame = (int)(dt / s_per_frame);
        time_start_prev_frame += add_frame * s_per_frame;
    }

    frame_now = (frame_now + add_frame) % frames;

    imgs[frame_now].Draw(canvas, p, flip_OX, flip_OY);
}