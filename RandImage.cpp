#include "RandImage.h"

const std::string png = ".png";

RandImage::RandImage(const std::string &path, int n)
{
    if (n < 1)error("n must be more than 0");
    pic_n = n;
    for (int _pic_n = 1; _pic_n <= n; _pic_n++) {
        images.emplace_back(path + "_" + std::to_string(_pic_n) + png);
        images_90.emplace_back(images[_pic_n - 1], E_ImgRotation::Rot_90);
    }
}

int RandImage::GetRandImageId() {return std::rand() % (pic_n * 2 * 4);}
int RandImage::GetRandImageId(bool rot_90) {return (std::rand() % (pic_n * 4)) + (rot_90 * pic_n * 4);}

void RandImage::Draw(int id, Image &canvas, Point p)
{
    auto &v = (id >= pic_n * 4) ? images_90 : images;
    id = id % (pic_n * 4);
    int v_id = id / 4;
    bool flip_OX = (id % 4) & 1;
    bool flip_OY = (id % 4) & 2;
    v[v_id].Draw(canvas, p, flip_OX, flip_OY);
}