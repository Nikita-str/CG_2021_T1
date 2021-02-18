#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

#include "General.h"
#include <algorithm>
#include <map>

Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, 4)) != nullptr)
  {
    size = width * height * channels;
  }
}

Image::Image(const std::string &a_path, int scale_x, int scale_y)
{
    auto temp = Image(a_path);
    ImgClone(temp, scale_x, scale_y);
}

Image::Image(const std::string &a_path, int scale) : Image(a_path, scale, scale) {}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * (size_t)a_height];

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}

Image::Image(const Image &copy) { ImgClone(copy, 1, 1); }

Image::Image(const Image &copy, int scale) : Image(copy, scale, scale) {}

void Image::ImgClone(const Image &copy, int scale_x, int scale_y)
{
    if (scale_x <= 0 || scale_y <= 0)std::abort();

    width = copy.width * scale_x;
    height = copy.height * scale_y;
    size = copy.size * scale_x * scale_y; 
    channels = copy.channels;

    if (scale_x == 1 && scale_y == 1) {
        data = new Pixel[width * (size_t)height];
        //for (int z = 0; z < width * height; z++)data[z] = copy.data[z];
        std::copy(copy.data, copy.data + (width * (size_t)height), data);
        self_allocated = true;
        return;
    }

    data = new Pixel[width * (size_t)height];

    int c_x = 0;
    int c_y = 0;
    for (int y = 0; y < height; y++, c_y += (y % scale_y == 0), c_x = 0)
        for (int x = 0; x < width; x++, c_x += (x % scale_x == 0))
            SetPixel(x, y, copy.GetPixel(c_x, c_y));

    self_allocated = true;
}

Image::Image(const Image &copy, int scale_x, int scale_y) { ImgClone(copy, scale_x, scale_y); }

Image::Image(Image&& a) noexcept : width(a.width), height(a.height), size(a.size), channels(a.channels), data(a.data), self_allocated(a.self_allocated)
{
    a.data = nullptr;
}


int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }

  return 0;
}

void Image::Draw(Image &canvas, Point p, bool flip_OX, bool flip_OY) const
{
    int c_w;
    int c_h;
    if (p.x <= -width                 || 
        p.y <= -height                || 
        p.x > (c_w = canvas.Width())  ||
        p.y > (c_h = canvas.Height())   ) return;

    int start_x = std::max(p.x, 0);
    int start_y = std::max(p.y, 0);

    int end_x = std::min(p.x + width, c_w);
    int end_y = std::min(p.y + height, c_h);

    int this_x_zero = (p.x < 0) ? -p.x : 0;
    int this_x = this_x_zero;
    int this_y = (p.y < 0) ? -p.y : 0;

    //for(int y = start_y; y < end_y; y++, this_x = this_x_zero, this_y++)
    //for(int x = start_x; x < end_x; x++, this_x++)
    //    canvas.MixPixel(x, y, GetPixel(this_x, this_y));

    #define Z(th_x, th_y)                                                \
    for(int y = start_y; y < end_y; y++, this_x = this_x_zero, this_y++) \
    for (int x = start_x; x < end_x; x++, this_x++)                      \
        canvas.MixPixel(x, y, GetPixel(th_x, th_y))

    if (!flip_OX && !flip_OY)Z(this_x, this_y);
    else if (flip_OX && !flip_OY)Z(this_x, height - 1 - this_y);
    else if (!flip_OX && flip_OY)Z(width - 1 - this_x, this_y);
    else /*if (flip_OY && flip_OX)*/Z(width - 1 - this_x, height - 1 - this_y);
    
    #undef Z
}

void Image::PixelsChange(std::function<Pixel(Pixel)> PixFunc, bool with_hash_pixel)
{
    if (!with_hash_pixel) {
        for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) 
            SetPixel(x, y, PixFunc(GetPixel(x, y)));
        return;
    }

    std::map<Pixel, Pixel> already {};
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            auto pix0 = GetPixel(x, y);
            auto p = already.find(pix0);
            if (p != already.end()) SetPixel(x, y, p->second);
            else {
                auto pix1 = PixFunc(pix0);
                already.insert({pix0, pix1});
                SetPixel(x, y, pix1);
            }
        }
}

Image::~Image()
{
    if (data == nullptr)return;

    if (self_allocated)
        delete[] data;
    else {
        stbi_image_free(data);
    }
}