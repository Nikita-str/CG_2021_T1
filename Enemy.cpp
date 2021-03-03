#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(Point center_pos, int _type) : type(_type), mov(center_pos)
{
    if (is_fly(type))mov.SetCanStay(true);
    mov.SetSize(get_mov_sz(_type));
    id = mov.GetId();
    max_hp = get_hp(_type);
    hp = max_hp;
    sz = get_sz(_type);
    speed = get_speed(_type);
    cur_dir = std::rand() % 2 ? E_Dir::LEFT : E_Dir::RIGHT;
    cur_state = E_LiveObjState::Idle;
}

void Enemy::Draw(Image &canvas)
{
    if (!alive)return;
    auto p = mov.CenterPos();
    p.y += 6;
    if (cur_state == E_LiveObjState::TakeHit && GameTime::Now().TimeCome(hit_take_time)) {
        cur_state = E_LiveObjState::Idle;
        if (hp <= 0)alive = false;
    }

    if (cur_state == E_LiveObjState::Attack && GameTime::Now().TimeCome(attack_end_time)) {
        //TODO:check player pos
        attack_cd = true;
        attack_cd_time = GameTime::Now().GetTime() + get_cd(type);
        cur_state = E_LiveObjState::Idle;
    }

    if (cur_state == E_LiveObjState::Walk) {
        if (!mov.Moved())cur_state = E_LiveObjState::Idle;
    }

    int hp_x = p.x - (max_hp / 20) * 12;
    for (int i = 0; i * 10 < max_hp; i ++) {
        SpriteManager::Get().DrawHpBlock(canvas, {.x = hp_x + i * 12, .y = p.y - sz.h / 2 - 5}, hp - i * 10);
    }

    SpriteManager::Get().enemy_spr[type].Draw(canvas, {.x = p.x - sz.w / 2, .y = p.y - sz.h / 2}, cur_state, cur_dir);
}

bool Enemy::IsCollide(Point pos, int border_sz)
{
    if (!alive)return false;
    auto p = mov.CenterPos();
    if (pos.x <= p.x - sz.w/2 + border_sz)return false;
    if (pos.x >= p.x + sz.w/2 - border_sz)return false;
    if (pos.y <= p.y - sz.h/2 + border_sz)return false;
    if (pos.y >= p.y + sz.h/2 - border_sz)return false;
    return true;
}


bool Enemy::WasAttacked(int dmg)
{
    if (!alive)return true;
    hp -= dmg;
    if (cur_state != E_LiveObjState::Attack) {
        cur_state = E_LiveObjState::TakeHit;
        hit_take_time = GameTime::Now().GetTime() + SpriteManager::Get().enemy_spr[type].GetAnimTime(cur_state);
    }
    return hp <= 0;
}

std::pair<bool, E_Dir> fn_can_attack(Player& pl, Point pos)
{
    if (pl.PointIn(pos + Point {TILE_SZ, 0}, 18)) return {true, E_Dir::RIGHT};
    if (pl.PointIn(pos + Point {-TILE_SZ, 0}, 18))return {true, E_Dir::LEFT};
    if (pl.PointIn(pos + Point {0, TILE_SZ}, 18)) return {true, E_Dir::UP};
    if (pl.PointIn(pos + Point {0, -TILE_SZ}, 18)) return {true, E_Dir::DOWN};
    return {false, E_Dir::DOWN};
}

void Enemy::Move(Point player_pos)
{
    if (!alive)return;
    int r = get_r(type);
    auto pos = mov.CenterPos();
    if ((cur_state == E_LiveObjState::TakeHit || cur_state == E_LiveObjState::Attack) ||
        (player_pos.x + r * TILE_SZ < pos.x) ||
        (player_pos.x - r * TILE_SZ > pos.x) ||
        (player_pos.y + r * TILE_SZ < pos.y) ||
        (player_pos.y - r * TILE_SZ > pos.y)) {
        mov.UpdateLastTime();
        return;
    }
    bool x_eq = std::abs(player_pos.x - pos.x) < 3;
    bool pl_x_less = player_pos.x < pos.x;


    bool y_eq = std::abs(player_pos.y - pos.y) < 3;
    bool pl_y_less = player_pos.y < pos.y;

    if (!x_eq)cur_dir = pl_x_less ? E_Dir::LEFT : E_Dir::RIGHT;

    auto &pl = Player::Get();
    if (!walk_to_player) {
        auto can_a = fn_can_attack(pl, pos);
        if (can_a.first) {
            walk_to_player = true; 
            time_when_try_attack = get_time_before_attack(type) + GameTime::Now().GetTime();
        }
    } else {
        if (GameTime::Now().TimeCome(time_when_try_attack)) {
            auto can_a = fn_can_attack(pl, pos);
            if (can_a.first) {
                attack_dir = can_a.second;
                cur_state = E_LiveObjState::Attack;
                attack_end_time = GameTime::Now().GetTime() + SpriteManager::Get().enemy_spr[type].GetAnimTime(cur_state);
            }
        }
    }

    mov.Move(x_eq ? 0 : pl_x_less ? -1 : 1, y_eq ? 0 : pl_y_less ? -1 : 1, speed);
    if (mov.Moved())cur_state = E_LiveObjState::Walk;
}