#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>
#include <functional>
#include "General.h"

constexpr int tileSize = 16;

#pragma region Pixel
struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  Pixel &operator+=(const Pixel &add)
  {
      if (add.a == 255) { r = add.r; g = add.g; b = add.b; return *this; }
      if (add.a == 0)return *this;
      double k = (add.a / 255.0);
      r = (uint8_t)((add.r - (int)r) * k + r);
      g = (uint8_t)((add.g - (int)g) * k + g);
      b = (uint8_t)((add.b - (int)b) * k + b);
      return *this;
  }

  inline bool operator< (Pixel const &other)
  {
      if (r != other.r) return r < other.r;
      if (g != other.g) return g < other.g;
      if (b != other.b) return b < other.b;
      return a < other.a;
  }
};

inline bool operator< (const Pixel a, const Pixel b)
{
    if (a.r != b.r) return a.r < b.r;
    if (a.g != b.g) return a.g < b.g;
    if (a.b != b.b) return a.b < b.b;
    return a.a < b.a;
}
#pragma endregion


constexpr Pixel backgroundColor{0, 0, 0, 0};

enum class E_ImgRotation { Rot_90 };

struct Image
{
  explicit Image(const std::string &a_path);
  Image(const std::string &a_path, int scale);
  Image(const std::string &a_path, int scale_x, int scale_y);

  Image(int a_width, int a_height, int a_channels);
  
  Image(const Image &copy, int scale);
  Image(const Image &copy, int scale_x, int scale_y);
  
  Image(const Image &copy, E_ImgRotation rot);

  Image(const Image &copy);
  Image(Image && a) noexcept;

  Image &operator=(const Image &other)
  {
      error("oh no...");
      return *this;
  }

  Image &operator=(Image &&other) noexcept
  {
      if (&other == this)return *this;
      width = other.width;
      height = other.height;
      size = other.size;
      channels = other.channels;
      self_allocated = other.self_allocated;

      data = other.data;
      other.data = nullptr;

      return *this;
  }

  int Save(const std::string &a_path);

  inline int Width()    const { return width; }
  inline int Height()   const { return height; }
  inline int Channels() const { return channels; }
  inline size_t Size()  const { return size; }
  inline Pixel* Data()        { return  data; }

  inline Pixel GetPixel(int x, int y) const { return data[width * y + x]; }

  inline void  MixPixel(int x, int y, const Pixel &pix) { data[width * y + x] += pix; }

  inline void  SetPixel(int x, int y, const Pixel &pix) { data[width * y + x] = pix; }

  void Draw(Image &canvas, Point p, bool flip_OX = false, bool flip_OY = false, bool not_mix = false) const;

  void FastDraw(Image &canvas, int lines, int from_line = 0) const;

  /// <summary>chnage image : image[x,y] = PixFunc(image[x,y])</summary>
  /// <param name="with_save_pixel">if we know that dif color on that image few then we can hash PixFunc result value</param>
  void PixelsChange(std::function<Pixel(Pixel)> PixFunc, bool with_hash_pixel);

  ~Image();

private:
  void ImgClone(const Image &copy, int scale_x, int scale_y); // for write wo copypast and wo temp obj 

  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  bool self_allocated = false;
};



#endif //MAIN_IMAGE_H
