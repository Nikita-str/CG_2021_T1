#include "Player.h"


bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(E_Dir dir)
{
  int move_dist = move_speed * 1;
  switch(dir)
  {
    case E_Dir::UP:
      old_coords.y = coords.y;
      coords.y += move_dist;
      break;
    case E_Dir::DOWN:
      old_coords.y = coords.y;
      coords.y -= move_dist;
      break;
    case E_Dir::LEFT:
      old_coords.x = coords.x;
      coords.x -= move_dist;
      break;
    case E_Dir::RIGHT:
      old_coords.x = coords.x;
      coords.x += move_dist;
      break;
    default:
      break;
  }

  spr.SetDir(dir);
}

void Player::Draw(Image &screen)
{
  spr.Draw(screen, coords, true);
}