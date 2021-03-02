#include "Enemy.h"

Enemy::Enemy(Point center_pos, int _type) : type(_type), mov(center_pos)
{
    hp = get_hp(_type);
    sz = get_sz(_type);
    speed = get_speed(_type);
    cur_dir = std::rand() % 2 ? E_Dir::LEFT : E_Dir::RIGHT;
}

void Enemy::Draw(Image &canvas)
{
    auto p = mov.Pos();
    SpriteManager::Get().enemy_spr[type].Draw(canvas, {.x = p.x - sz.w / 2, .y = p.y - sz.h / 2}, cur_dir);
}