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

struct MapObj
{
    MapObj(GameMap &_parent) : items(), parent(_parent) {};
    //void AddItem(Point pos, int rarity);
    void AddKey(Point pos);
    void AddItem(Point pos, int lvl);
    void AddEnemy(Point pos, int type);
    
    void PressE();

    void DrawItems(Image &canvas);
    void DrawEnemies(Image &canvas);
    std::vector<Item> &GetItems() { return items; }
private:
    std::vector<Item> items {};
    std::vector<Enemy> enemies {};
    GameMap &parent;
    int ind_E = -1;
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

    bool CanStay(E_CanStayType stay_type, Point pos, Size obj_sz, bool empty_can)
    {
        Point LD {.x = pos.x, .y = pos.y};
        Point LU {.x = pos.x, .y = pos.y + obj_sz.h - 1};
        Point RU {.x = pos.x + obj_sz.w - 1, .y = pos.y + obj_sz.h - 1};
        Point RD {.x = pos.x + obj_sz.w - 1, .y = pos.y};

        switch (stay_type) {
        case GameMap::E_CanStayType::Right:
            return CanPointStay(RU, empty_can) && CanPointStay(RD, empty_can);
        case GameMap::E_CanStayType::Left:
            return CanPointStay(LD, empty_can) && CanPointStay(LU, empty_can);
        case GameMap::E_CanStayType::Up:
            return CanPointStay(LU, empty_can) && CanPointStay(RU, empty_can);
        case GameMap::E_CanStayType::Down:
            return CanPointStay(LD, empty_can) && CanPointStay(RD, empty_can);
        default:break;
        }
    }

    bool CanPointStay(Point pos, bool empty_can) const
    {
        auto t_type = now_room->gri.TileType(pos.x / TILE_SZ, pos.y / TILE_SZ);
        if (t_type == GameMap::E_TileType::Wall)return false;
        if (!empty_can && t_type == GameMap::E_TileType::Empty)return false;
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

    std::vector<Item> &GetItems() { return now_room->map_objects.GetItems(); }

    bool CanThrowItem(Point pos);

private:
    void load_room(int x, int y);

    struct GameRoomInfo // general, lfrom that you need copy data into other class, that changeble it's staitc info!
    {
        GameRoomInfo(char room_type);
        std::vector<std::vector<GameMap::E_TileType>> tile_type{};
        std::vector<std::pair<Point, int>> enemies{}; // point - pos, int - type
        std::vector<Point> door_pos{}; // point - pos
        std::vector< std::pair<Point, int>> items{}; // point - pos,  int - rarity
        std::vector<Point> keys_pos{}; // point - pos
        int map_width = -1;
        int map_height = -1;

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