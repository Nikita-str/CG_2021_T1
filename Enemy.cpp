#include "Enemy.h"

Enemy::Enemy(Point center_pos, int _type) : spr(E_LiveObjType::Enemy, _type, get_move_frames(_type), get_attack_frames(_type)), mov(center_pos)
{
    hp = get_hp(_type);
    sz = get_sz(_type);
    speed = get_speed(_type);
}

void Enemy::Draw(Image &canvas)
{
    auto p = mov.Pos();
    spr.Draw(canvas, {.x = p.x - sz.w / 2, .y = p.y - sz.h / 2});
}