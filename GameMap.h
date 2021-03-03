#ifndef MAIN_Gmap_H
#define MAIN_Gmap_H

#include <vector>
#include <map>
#include <string>

#include "General.h"
#include "Image.h"
#include "RandImage.h"


struct GameMap;
#include "Item.h"
#include "Enemy.h"

struct Door
{
    Point pos;
    bool open;
    E_Dir dir;
    Door(Point _pos, E_Dir _dir): pos(_pos), open(false), dir(_dir){}
    Door(Point _pos, bool _open, E_Dir _dir): pos(_pos), open(_open), dir(_dir) {}
    bool IsRectIn(Point p, Size sz)
    {
        if (open)return false;
        int w = TILE_SZ;
        int h = TILE_SZ;
        int p_x = pos.x * TILE_SZ + (TILE_SZ - w) / 2;
        int p_y = pos.y * TILE_SZ + (TILE_SZ - h) / 2;
        if (p.x < p_x && p.x + sz.w < p_x)return false;
        if (p.x > p_x && p.x > p_x + w)return false;
        if (p.y < p_y && p.y + sz.h < p_y)return false;
        if (p.y > p_y && p.y > p_y + h)return false;
        return true;
    }
};

struct MapObj
{
    MapObj(GameMap &_parent) : items(), parent(_parent) {};
    //void AddItem(Point pos, int rarity);
    void AddKey(Point pos);
    void AddItem(Point pos, int lvl);
    void AddEnemy(Point pos, int type);
    void AddDoor(Point pos, E_Dir dir, bool open = false) { doors.emplace_back(pos, open, dir); };
    
    void PressE();

    void DrawItems(Image &canvas);
    void DrawEnemies(Image &canvas);
    std::vector<Item> &GetItems() { return items; }

    bool CanStay(Point pos);

    bool TryAttack(Point pos, int dmg);
private:
    std::vector<Item> items {};
    std::vector<Enemy> enemies {};
    std::vector<Door> doors {};
    GameMap &parent;
    int ind_E = -1;
    int ind_D = -1;
};




struct GameMap
{
private: static GameMap *cur_map;
public:
    static GameMap *GetCur() { return cur_map; }

    enum class E_MapPos
    {
        Center,
    };

    enum class E_TileType
    {
        Floor,
        Wall,
        Empty,
        Nothing
    };

    enum class E_CanStayType
    {
        Right,
        Left,
        Up,
        Down
    };

    GameMap();

    bool CheckChangeMap();

    bool CanStay(E_CanStayType stay_type, Point pos, Size obj_sz, bool empty_can)
    {
        Point LD {.x = pos.x, .y = pos.y};
        Point LU {.x = pos.x, .y = pos.y + obj_sz.h - 1};
        Point RU {.x = pos.x + obj_sz.w - 1, .y = pos.y + obj_sz.h - 1};
        Point RD {.x = pos.x + obj_sz.w - 1, .y = pos.y};

        Point R_UD {.x = pos.x + obj_sz.w - 1, .y = pos.y + obj_sz.h / 2};
        Point L_UD {.x = pos.x,                .y = pos.y + obj_sz.h / 2};
        Point LR_U {.x = pos.x + obj_sz.w / 2, .y = pos.y + obj_sz.h - 1};
        Point LR_D {.x = pos.x + obj_sz.w / 2, .y = pos.y};

        switch (stay_type) {
        case GameMap::E_CanStayType::Right:
            return CanPointStay(RU, empty_can) && CanPointStay(RD, empty_can) && now_room->map_objects.CanStay(R_UD);
        case GameMap::E_CanStayType::Left:
            return CanPointStay(LD, empty_can) && CanPointStay(LU, empty_can) && now_room->map_objects.CanStay(L_UD);
        case GameMap::E_CanStayType::Up:
            return CanPointStay(LU, empty_can) && CanPointStay(RU, empty_can) && now_room->map_objects.CanStay(LR_U);
        case GameMap::E_CanStayType::Down:
            return CanPointStay(LD, empty_can) && CanPointStay(RD, empty_can) && now_room->map_objects.CanStay(LR_D);
        default: error("wrong enum state"); return false; 
        }
    }

    bool CanPointStay(Point pos, bool empty_can) const
    {
        auto t_type = now_room->gri.TileType(pos.x / TILE_SZ, pos.y / TILE_SZ);
        if (t_type == GameMap::E_TileType::Wall)return false;
        if (!empty_can && t_type == GameMap::E_TileType::Empty)return false;
        if (!now_room->map_objects.CanStay(pos))return false;
        return true;
    }

    GameMap::E_TileType PointType(Point pos) const { return now_room->gri.TileType(pos.x / TILE_SZ, pos.y / TILE_SZ); }

    Point GetPos(E_MapPos map_pos, Size obj_sz);

    void Draw(Image &canvas);
    void Draw_E(Image &canvas, Point p)
    {
        key_E_img.Draw(canvas, {.x = p.x * TILE_SZ + (TILE_SZ - key_E_img.Width()) / 2, .y = (p.y + 1) * TILE_SZ}, true);
    }

    int PixWidth()const { return now_room->gri.map_width * TILE_SZ; }
    int PixHeight()const { return now_room->gri.map_height * TILE_SZ; }

    template <typename T> void DrawOn(T &img_for_draw, Point map_pos, Image &canvas)
    {
        int map_x = map_pos.x;
        int map_y = map_pos.y;
        img_for_draw.Draw(canvas, {.x = map_x * TILE_SZ + (TILE_SZ - img_for_draw.Width()) / 2, .y = map_y * TILE_SZ + (TILE_SZ - img_for_draw.Height()) / 2}, true);
    }

    void PressE() { now_room->map_objects.PressE(); }

    bool TryAttack(Point pos, int dmg) { return now_room->map_objects.TryAttack(pos, dmg); }

    std::vector<Item> &GetItems() { return now_room->map_objects.GetItems(); }

    bool CanThrowItem(Point pos);

private:
    void load_room(int x, int y);

    struct GameRoomInfo // general, lfrom that you need copy data into other class, that changeble it's staitc info!
    {
        static constexpr int U = 0;
        static constexpr int L = 1;
        static constexpr int R = 2;
        static constexpr int D = 3;
        static constexpr Point BAD_IN = Point {-1, -1};
        GameRoomInfo(char room_type);
        std::vector<std::vector<GameMap::E_TileType>> tile_type{};
        std::vector<std::pair<Point, int>> enemies{}; // point - pos, int - type
        std::vector<Point> door_pos{}; // point - pos
        std::vector< std::pair<Point, int>> items{}; // point - pos,  int - rarity
        std::vector<Point> keys_pos{}; // point - pos
        int map_width = -1;
        int map_height = -1;

        std::vector<Point> point_in {};

        GameMap::E_TileType TileType(int x, int y) const
        {
            if (x < 0 || map_width <= x)return GameMap::E_TileType::Nothing;
            if (y < 0 || map_height <= y)return GameMap::E_TileType::Nothing;
            return tile_type[map_height - y - 1][x];
        }
    };

    struct GameRoom
    {
        std::vector<std::vector<int>> map_ids{};
        GameMap::GameRoomInfo &gri;
        Image room_holst;
        GameRoom(GameMap::GameRoomInfo &_gri, GameMap &_parent);
        MapObj map_objects;
    private:
        GameMap &parent;
    };

    std::vector<std::string> room_map {};
    std::map<Point, GameRoom> loaded_room{};
    std::map<char, GameRoomInfo> loaded_room_info {};

    RandImage floor_img;
    Image empty_img;
    Image wall_img;
    Sprite key_E_img;

    GameMap::GameRoom *now_room = nullptr;
    
    int now_x = 0;
    int now_y = 0;
};

#endif