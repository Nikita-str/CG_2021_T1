#include "Enemy.h"

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
    cur_state = E_LiveObjState::TakeHit;
    hit_take_time = GameTime::Now().GetTime() + SpriteManager::Get().enemy_spr[type].GetAnimTime(cur_state);
    return hp <= 0;
}

void Enemy::Move(Point player_pos)
{
    if (!alive)return;
    int r = get_r(type);
    auto pos = mov.CenterPos();
    if ((player_pos.x + r * TILE_SZ < pos.x) ||
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

    mov.Move(x_eq ? 0 : pl_x_less ? -1 : 1, y_eq ? 0 : pl_y_less ? -1 : 1, speed);
}