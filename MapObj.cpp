#include "GameMap.h"
#include "Player.h"

#include <iterator>
#include <utility>

void MapObj::AddKey(Point pos){items.emplace_back(E_ItemTypes::Key, -1, pos);}
void MapObj::AddItem(Point pos, int lvl){items.emplace_back(lvl, pos);}
void MapObj::AddEnemy(Point pos, int type) { enemies.emplace_back(pos * TILE_SZ + TILE_2, type); }

void MapObj::DrawItems(Image &canvas)
{
    E_Dir pl_dir = Player::Get().GetCurDir();
    auto pl_c = Player::Get().GetCenter();

    int ind_e = -1;

    Point pl_check_min {0,0};
    Size pl_check_sz {15,15};

    switch (pl_dir) {
    case E_Dir::UP: pl_check_min = {.x = pl_c.x - 10, .y = pl_c.y - 12}; pl_check_sz.w += 5; break;
    case E_Dir::DOWN: pl_check_min = {.x = pl_c.x - 10, .y = pl_c.y - 16}; pl_check_sz.w += 5; break;
    case E_Dir::LEFT: pl_check_min = {.x = pl_c.x - 15, .y = pl_c.y - 10}; pl_check_sz.h += 5; break;
    case E_Dir::RIGHT: pl_check_min = {.x = pl_c.x + 5, .y = pl_c.y - 10};  pl_check_sz.h += 5; break;
    default:break;
    }

    bool check_points = true;
    for (int i = 0; i < items.size(); i++) {
        auto &itm = items[i];
        if (check_points) {
            if (itm.IsRectIn(pl_check_min, pl_check_sz)) {
                parent.Draw_E(canvas, itm.pos); 
                check_points = false; 
                ind_e = i; 
            }
        }
        parent.DrawOn<Sprite>(itm.spr, itm.pos, canvas);
    }

    ind_E = ind_e;
}


void MapObj::DrawEnemies(Image &canvas)
{
    for (int i = 0; i < enemies.size(); i++)
        enemies[i].Draw(canvas);
}

void MapObj::PressE()
{
    if (ind_E < 0)return;
    auto &itm = items[ind_E];
    if (!itm.ict())return;
    itm.ia(true);
    if (itm.inventory) {
        auto &inv = Player::Get().GetInv();
        //inv.insert(inv.end(), std::make_move_iterator(items.begin() + ind_E), std::make_move_iterator(items.begin() + ind_E + 1));
        inv.push_back(std::move(itm));
        items.erase(items.begin() + ind_E);
    } else {
        items.erase(items.begin() + ind_E);
    }
}
