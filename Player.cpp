#include "Player.h"


bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::RefreshMoveState(E_Dir dir)
{
    int move_dist = move_speed * 1;
    switch (dir) {
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
    default:break;
    }
}

void Player::RefreshMoveState(E_X_Dir x, E_Y_Dir y)
{
    bool x_move = x != E_X_Dir::Not;
    bool y_move = y != E_Y_Dir::Not;

    if (x_move && y_move) {
        bool change_dir = false;
        
        E_Dir e_dir_x = (x == E_X_Dir::Left) ? E_Dir::LEFT : E_Dir::RIGHT;
        E_Dir e_dir_y = (y == E_Y_Dir::Up) ? E_Dir::UP : E_Dir::DOWN;
        RefreshMoveState(e_dir_x);
        RefreshMoveState(e_dir_y);

        if(e_dir_x != back_dir && e_dir_y != back_dir)
            spr.SetDir(Is_X_Dir(back_dir) ? e_dir_y : e_dir_x);
    }
    else if (x_move) {
        E_Dir e_dir_x = (x == E_X_Dir::Left) ? E_Dir::LEFT : E_Dir::RIGHT;
        RefreshMoveState(e_dir_x);
        spr.SetDir(e_dir_x);
        back_dir = e_dir_x;
    }
    else if (y_move) {
        E_Dir e_dir_y = (y == E_Y_Dir::Up) ? E_Dir::UP : E_Dir::DOWN;
        RefreshMoveState(e_dir_y);
        spr.SetDir(e_dir_y);
        back_dir = e_dir_y;
    }

    //TODO : if (NOT ATTACKED)
    spr.SetState(Moved() ? E_LiveObjState::Walk : E_LiveObjState::Idle);
    //printf(Moved() ? "move\n" : "not move\n");

}

void Player::Draw(Image &screen)
{
    old_coords = coords;
    spr.Draw(screen, coords, true);
}