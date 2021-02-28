#include "GameMap.h"
#include "Player.h"

void MapObj::AddKey(Point pos)
{
    items.emplace_back(pos, Sprite("../resources/key.png", 1), [](bool on) {if (on)Player::Get().KeyInc(); else Player::Get().KeyDec(); });
}

void MapObj::DrawItems(Image &canvas)
{
    E_Dir pl_dir = Player::Get().GetCurDir();
    auto pl_c = Player::Get().GetCenter();

    Point check_p_0 {0,0};
    Point check_p_1 {0,0};
    Point check_p_2 {0,0};
    Point check_p_3 {0,0};

    Point pl_p_min {0,0};
    Point pl_p;

    switch (pl_dir) {
    case E_Dir::UP:
        check_p_0 = {.x = (pl_c.x - 10), .y = pl_c.y};
        check_p_1 = {.x = (pl_c.x + 10), .y = check_p_0.y};
        check_p_2 = {.x = check_p_0.x, .y = (pl_c.y + 20)};
        check_p_3 = {.x = check_p_1.x, .y = check_p_2.y};
        break;
    case E_Dir::DOWN:
        check_p_0 = {.x = (pl_c.x - 10), .y = pl_c.y};
        check_p_1 = {.x = (pl_c.x + 10), .y = check_p_0.y};
        check_p_2 = {.x = check_p_0.x, .y = (pl_c.y - 20)};
        check_p_3 = {.x = check_p_1.x, .y = check_p_2.y};
        break;
    case E_Dir::LEFT:
        check_p_0 = {.x = (pl_c.x), .y = (pl_c.y - 10)};
        check_p_1 = {.x = check_p_0.x, .y = (pl_c.y + 10)};
        check_p_2 = {.x = (pl_c.x + 20), .y = check_p_0.y };
        check_p_3 = {.x = check_p_2.x, .y = check_p_1.y};
        break;
    case E_Dir::RIGHT:
        check_p_0 = {.x = (pl_c.x), .y = (pl_c.y - 10)};
        check_p_1 = {.x = check_p_0.x, .y = (pl_c.y + 10)};
        check_p_2 = {.x = (pl_c.x - 20), .y = check_p_0.y};
        check_p_3 = {.x = check_p_2.x, .y = check_p_1.y};
        break;
    default:break;
    }

    bool check_points = true;
    for (int i = 0; i < items.size(); i++) {
        auto &itm = items[i];
        if (check_points) {
            if (itm.IsPointIn(check_p_0) || itm.IsPointIn(check_p_1) || 
                itm.IsPointIn(check_p_2) || itm.IsPointIn(check_p_3)) {
                parent.Draw_E(canvas, itm.pos); 
                check_points = false; 
            }
        }
        parent.DrawOn<Sprite>(itm.spr, itm.pos, canvas);
    }
}

