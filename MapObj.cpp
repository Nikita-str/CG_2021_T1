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
    int ind_d = -1;

    Point pl_check_min {0,0};
    Size pl_check_sz {15,15};

    switch (pl_dir) {
    case E_Dir::UP: pl_check_min = {.x = pl_c.x - 10, .y = pl_c.y - 12}; pl_check_sz.w += 5; break;
    case E_Dir::DOWN: pl_check_min = {.x = pl_c.x - 10, .y = pl_c.y - 16}; pl_check_sz.w += 5; break;
    case E_Dir::LEFT: pl_check_min = {.x = pl_c.x - 15, .y = pl_c.y - 10}; pl_check_sz.h += 5; break;
    case E_Dir::RIGHT: pl_check_min = {.x = pl_c.x + 5, .y = pl_c.y - 10};  pl_check_sz.h += 5; break;
    default:break;
    }

    Point pl_check_door_min = pl_check_min;
    if (pl_dir == E_Dir::UP)pl_check_door_min.y += 24;

    bool check_points = true;
    bool player_have_key = Player::Get().HaveKey();
    for (int i = 0; i < doors.size(); i++) {
        auto &door= doors[i];
        if (player_have_key && check_points) {
            if (door.IsRectIn(pl_check_door_min, pl_check_sz)) {
                parent.Draw_E(canvas, door.pos);
                check_points = false;
                ind_d = i; 
            }
        }
        if(!door.open)SpriteManager::Get().DrawDoor(canvas, door.pos, door.dir);
    }

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
    ind_D = ind_d;
}


void MapObj::DrawEnemies(Image &canvas)
{
    for (int i = 0; i < enemies.size(); i++)
        enemies[i].Draw(canvas);
}

void MapObj::PressE()
{
    if (ind_D != -1) {
        if (Player::Get().HaveKey())Player::Get().KeyDec();
        doors[ind_D].open = true;
        ind_D = -1;
        return;
    }
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
    ind_E = -1;
}

bool MapObj::CanStay(Point pos)
{
    for (int i = 0; i < doors.size(); i++) 
        if (!doors[i].open && (doors[i].pos == pos / TILE_SZ))return false;
    
    for (int i = 0; i < enemies.size(); i++)if (enemies[i].IsCollide(pos, 5))return false;
    return true;
}

bool MapObj::TryAttack(Point pos, int dmg)
{
    for (int i = 0; i < enemies.size(); i++) {
        auto &e = enemies[i];
        if (e.IsCollide(pos, 0)) {
            e.WasAttacked(dmg);//die
            return true;
        }
    }
    return false;
}