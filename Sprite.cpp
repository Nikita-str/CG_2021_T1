#include "Sprite.h"
#include <iostream>
#include <utility>

void Sprite::SpriteFromImg(Image &img0, int p_frames, int ms_on_frame, int scale)
{
    frames = p_frames;
    s_per_frame = ms_on_frame / 1000.0;

    if (p_frames <= 0)
    {
        std::cerr << "too few amount of frames : " << p_frames << std::endl;
        std::abort();
    }

    if (frames == 1)
    {
        imgs.emplace_back(img0);
        return;
    }

    int w = img0.Width();
    int h = img0.Height();
    int chans = img0.Channels();

    int sz_w = w / frames;
    int sz_h = h;

    int temp_x_now = 0;

    for (int frame = 0; frame < frames; frame++, temp_x_now -= sz_w)
    {
        imgs.emplace_back(sz_w, sz_h, chans);
        img0.Draw(imgs[frame], {.x = temp_x_now, .y = 0}, false, false, true);
    }
}

Sprite::Sprite(const std::string &path, int p_frames, int ms_on_frame, int scale, bool _loop) : imgs(), loop(_loop)
{
    auto im_gg = Image{path, scale};
    auto &img_ptr = im_gg;
    SpriteFromImg(img_ptr, p_frames, ms_on_frame, scale);
}

Sprite::Sprite(const std::string &path, SpritePixSz frame_sz, int ms_on_frame, int scale, bool _loop) : imgs(), loop(_loop)
{
    auto img0 = Image{path, scale};
    if (frame_sz.width == 0)
        std::abort();
    int p_frames = img0.Width() / (frame_sz.width * scale);
    SpriteFromImg(img0, p_frames, ms_on_frame, scale);
}

Sprite::Sprite(Image &img, SpritePixSz frame_sz, int ms_on_frame, int scale, bool _loop) : imgs(), loop(_loop)
{
    if (frame_sz.width == 0)
        error("sz need to be more than 0");
    int p_frames = img.Width() / (frame_sz.width * scale);
    SpriteFromImg(img, p_frames, ms_on_frame, scale);
}

void Sprite::Draw(Image &canvas, const Point p, double time_start, bool flip_OX, bool flip_OY)
{
    if (frames == 0)
        return;
    double dt = GameTime::Now().GetSecAfter(time_start);
    if (dt < 0)
        dt = 0;
    int _frame = (int)(dt / s_per_frame);
    imgs[_frame % frames].Draw(canvas, p, flip_OX, flip_OY);
}

void Sprite::Draw(Image &canvas, const Point p, bool flip_OX, bool flip_OY)
{
    if (frames == 0)
        return;
    double dt = GameTime::Now().GetSecAfter(time_start_prev_frame);
    if (dt < 0)
        dt = 0;

    int add_frame = 0;

    if (time_start_prev_frame < 0)
    {
        time_start_prev_frame = GameTime::Now().GetTime();
        add_frame = 0;
        frame_now = 0;
    }
    else if (dt > s_per_frame)
    {
        add_frame = (int)(dt / s_per_frame);
        time_start_prev_frame += add_frame * s_per_frame;
    }

    if (loop)
    {
        frame_now = (frame_now + add_frame) % frames;
    }
    else
    {
        frame_now = frame_now + add_frame;
        if (frame_now >= frames)
            frame_now = frames - 1;
    }

    imgs[frame_now].Draw(canvas, p, flip_OX, flip_OY);
}