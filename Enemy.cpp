#include "Enemy.h"

Enemy::Enemy(Point center_pos, int _type) : type(_type), mov(center_pos + Point{.x = 0, .y = 5})
{
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
    auto p = mov.Pos();
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
    auto p = mov.Pos();
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
