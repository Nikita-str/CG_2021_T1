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
    Sprite() {};
    Sprite(const std::string &path, int p_frames, int ms_on_frame = 125, int scale = 1);
    Sprite(const std::string &path, SpritePixSz frame_sz, int ms_on_frame = 125, int scale = 1);

    void Draw(Image &canvas, const Point p, bool flip_OX = false, bool flip_OY = false);

    void Restart() { time_start_prev_frame = GameTime::Now().GetTime(); frame_now = 0; }

    double AnimTime() const { return s_per_frame * frames; }

    int Width()const { return frames == 0 ? frames : imgs[frame_now].Width(); }
    int Height()const { return frames == 0 ? frames : imgs[frame_now].Height(); }

    const Image &GetFrame(int frame) const { return imgs.at(frame); }
private:
    void SpriteFromImg(Image &img, int p_frames, int ms_on_frame = 125, int scale = 1);

    int frames = 0;
    int frame_now = 0;
    double s_per_frame = 0.075;

    double time_start_prev_frame = -1;
    std::vector<Image> imgs;
};

#endif