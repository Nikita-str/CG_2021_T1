#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "LiveObjSprite.h"
#include "General.h"

struct Player
{
  //explicit Player(Point pos = {.x = 10, .y = 10}) : coords(pos), old_coords(coords) {};

  explicit Player(Point pos, LiveObjSprite&sprite) : coords(pos), old_coords(coords), spr(sprite) {};

  bool Moved() const;
  void ProcessInput(E_Dir dir);
  void Draw(Image &screen);

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  int move_speed = 4;

  LiveObjSprite &spr;

};

#endif //MAIN_PLAYER_H
