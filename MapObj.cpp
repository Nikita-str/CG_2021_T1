#include "GameMap.h"
#include "Player.h"

void MapObj::AddKey(Point pos)
{
    items.emplace_back(pos, Sprite("../resources/key.png", 1), [](bool on) {if (on)Player::Get().KeyInc(); else Player::Get().KeyDec(); });
}

void MapObj::DrawItems(Image &canvas)
{
    for (int i = 0; i < items.size(); i++)parent.DrawOn<Sprite>(items[i].spr, items[i].pos, canvas);
}

