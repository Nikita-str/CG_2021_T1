#include "Player.h"

Player *Player::player_getter;

void Player::Attack()
{
    if (spr.SetState(E_LiveObjState::Attack)) {
        blocked_to_time = GameTime::Now().GetTime() + spr.GetCurAnimTime();
        now_attack = true;

        auto dir = GetCurDir();
        auto cp = position.CenterPos();
        switch (dir) {
        case E_Dir::UP:
        {
            bool b = GameMap::GetCur()->TryAttack({cp.x, cp.y + 25}, damage);
            if (!b)b = GameMap::GetCur()->TryAttack({cp.x - 10, cp.y + 25}, damage);
            if (!b)b = GameMap::GetCur()->TryAttack({cp.x + 10, cp.y + 25}, damage);
            break;
        }
        case E_Dir::DOWN:
        {
            bool b = GameMap::GetCur()->TryAttack({cp.x, cp.y - 25}, damage);
            if(!b)b = GameMap::GetCur()->TryAttack({cp.x - 10, cp.y - 25}, damage);
            if(!b)b = GameMap::GetCur()->TryAttack({cp.x + 10, cp.y - 25}, damage);
            break;
        }
        case E_Dir::LEFT:
            GameMap::GetCur()->TryAttack({cp.x - 25, cp.y}, damage);
            break;
        case E_Dir::RIGHT:
            GameMap::GetCur()->TryAttack({cp.x + 25, cp.y}, damage);
            break;
        }
    }
}

bool Player::Moved() { return !now_attack && position.Moved(); }

void Player::RefreshMoveState(E_X_Dir x, E_Y_Dir y, bool ctrl)
{
    if (now_attack) { position.UpdateLastTime(); return; }

    old_coords = coords;

    position.Move((x == E_X_Dir::Not) ? 0 : ((x == E_X_Dir::Right) ? 1 : -1),
                  (y == E_Y_Dir::Not) ? 0 : ((y == E_Y_Dir::Up)    ? 1 : -1), (ctrl) ? move_speed_ctrl : move_speed);

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
    CheckStayOnEmpty(coords);
    //printf(Moved() ? "move\n" : "not move\n");
}

void Player::DieDraw(Image &canvas, double proc)
{
    if (!died)return;
    if (die_type == E_DieType::EmptyStay) {
        SetPosY(-30 + -50 + (50 + die_pos.y) * (1 - proc));
        spr.GetImage(E_LiveObjState::Idle, 0).Draw(canvas, coords, false);
    } else {
        grave.Draw(canvas, coords, true);
    }
    //for kill draw grave
}

void Player::Draw(Image &screen)
{
    if (died)return;
    if (now_attack && GameTime::Now().TimeCome(blocked_to_time)) {
        now_attack = false;
        spr.SetState(E_LiveObjState::Idle);
    }
    spr.Draw(screen, coords, true);
    
}