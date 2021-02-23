#ifndef MAIN_RAIMG_H
#define MAIN_RAIMG_H

#include <string>
#include <vector>
#include "Image.h"

struct RandImage
{
    RandImage(const std::string &path, int n);
    int GetRandImageId();
    int GetRandImageId(bool rot_90);
    void Draw(int id, Image &canvas, Point p);
private:
    std::vector<Image> images {};
    std::vector<Image> images_90 {};
    int pic_n {0};
};

#endif

