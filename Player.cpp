#include "Player.h"


bool Player::Moved() { return position.Moved(); }

void Player::RefreshMoveState(E_X_Dir x, E_Y_Dir y)
{
    old_coords = coords;

    position.Move((x == E_X_Dir::Not) ? 0 : ((x == E_X_Dir::Right) ? 1 : -1),
                  (y == E_Y_Dir::Not) ? 0 : ((y == E_Y_Dir::Up)    ? 1 : -1), move_speed);

    bool x_move = x != E_X_Dir::Not;
    bool y_move = y != E_Y_Dir::Not;

    if (x_move && y_move) {
        E_Dir e_dir_x = (x == E_X_Dir::Left) ? E_Dir::LEFT : E_Dir::RIGHT;
        E_Dir e_dir_y = (y == E_Y_Dir::Up) ? E_Dir::UP : E_Dir::DOWN;

        if(e_dir_x != back_dir && e_dir_y != back_dir)
            spr.SetDir(Is_X_Dir(back_dir) ? e_dir_y : e_dir_x);
    }
    else if (x_move) {
        E_Dir e_dir_x = (x == E_X_Dir::Left) ? E_Dir::LEFT : E_Dir::RIGHT;
        spr.SetDir(e_dir_x);
        back_dir = e_dir_x;
    }
    else if (y_move) {
        E_Dir e_dir_y = (y == E_Y_Dir::Up) ? E_Dir::UP : E_Dir::DOWN;
        spr.SetDir(e_dir_y);
        back_dir = e_dir_y;
    }

    coords = position.Pos();

    //TODO : if (NOT ATTACKED)
    spr.SetState(Moved() ? E_LiveObjState::Walk : E_LiveObjState::Idle);
    //printf(Moved() ? "move\n" : "not move\n");
}

void Player::Draw(Image &screen)
{
    spr.Draw(screen, coords, true);
}