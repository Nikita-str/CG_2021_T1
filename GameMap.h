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
struct MapObj
{
    MapObj(GameMap &_parent) : items(), parent(_parent) {};
    //void AddItem(Point pos, int rarity);
    void AddKey(Point pos);

    void DrawItems(Image &canvas);
private:
    std::vector<Item> items {};
    GameMap &parent;
};


static constexpr int TILE_SZ = 32;


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
        Point LU {.x = pos.x, .y = pos.y + obj_sz.h};
        Point RU {.x = pos.x + obj_sz.w, .y = pos.y + obj_sz.h};
        Point RD {.x = pos.x + obj_sz.w, .y = pos.y};

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

    bool CanPointStay(Point pos, bool empty_can)
    {
        auto t_type = now_room->gri.TileType(pos.x / TILE_SZ, pos.y / TILE_SZ);
        if (t_type == GameMap::E_TileType::Wall)return false;
        if (!empty_can && t_type == GameMap::E_TileType::Empty)return false;
        return true;
    }

    //TODO: del Move[X|Y]
    int MoveX(Point back_pos, int now_x, Size obj_sz)
    {
        if (back_pos.x == now_x)return now_x;
        else {
            int block_ny = (back_pos.y) / TILE_SZ;
            bool dif_ny = block_ny != ((back_pos.y + obj_sz.h - 1) / TILE_SZ);

            if (back_pos.x < now_x) { // --->  
                int block_nx = (now_x + obj_sz.w) / TILE_SZ;
                if (block_nx < 0 || block_nx >= now_room->gri.map_width)return now_x;
                if (now_room->gri.TileType(block_nx, block_ny) == GameMap::E_TileType::Wall ||
                    (dif_ny && now_room->gri.TileType(block_nx, block_ny + 1) == GameMap::E_TileType::Wall))
                    return back_pos.x; // (block_nx * TILE_SZ) - obj_sz.w;
                else return now_x;
            } else { // <---
                int block_nx = (now_x) / TILE_SZ;
                if (block_nx < 0 || block_nx >= now_room->gri.map_width)return now_x;
                if (now_room->gri.TileType(block_nx, block_ny) == GameMap::E_TileType::Wall ||
                    (dif_ny && now_room->gri.TileType(block_nx, block_ny + 1) == GameMap::E_TileType::Wall))
                    return back_pos.x; // ((block_nx + 1) * TILE_SZ);
                else return now_x;
            }
        }
    }

    int MoveY(Point back_pos, int now_y, Size obj_sz)
    {
        if (back_pos.y == now_y)return now_y;
        else {
            int block_nx = (back_pos.x) / TILE_SZ;
            bool dif_nx = block_nx != ((back_pos.x + obj_sz.w - 1) / TILE_SZ);

            if (back_pos.y < now_y) { // ^  
                int block_ny = (now_y + obj_sz.h) / TILE_SZ;
                if (block_ny < 0 || block_ny >= now_room->gri.map_height)return now_y;
                if (now_room->gri.TileType(block_nx, block_ny) == GameMap::E_TileType::Wall ||
                    (dif_nx && now_room->gri.TileType(block_nx + 1, block_ny) == GameMap::E_TileType::Wall))
                    return back_pos.y; // (block_ny * TILE_SZ) - obj_sz.h;
                else return now_y;
            } else { // V
                int block_ny = (now_y) / TILE_SZ;
                if (block_ny < 0 || block_ny >= now_room->gri.map_height)return now_y;
                if (now_room->gri.TileType(block_nx, block_ny) == GameMap::E_TileType::Wall ||
                    (dif_nx && now_room->gri.TileType(block_nx + 1, block_ny) == GameMap::E_TileType::Wall))
                    return back_pos.y; // ((block_ny + 1) * TILE_SZ);
                else return now_y;
            }
        }
    }

    Point GetPos(E_MapPos map_pos, Size obj_sz);

    void Draw(Image &canvas);

    int PixWidth()const { return now_room->gri.map_width * TILE_SZ; }
    int PixHeight()const { return now_room->gri.map_height * TILE_SZ; }

    template <typename T> void DrawOn(T &img_for_draw, Point map_pos, Image &canvas)
    {
        int map_x = map_pos.x;
        int map_y = map_pos.y;
        img_for_draw.Draw(canvas, {.x = map_x * TILE_SZ + (TILE_SZ - img_for_draw.Width()) / 2, .y = map_y * TILE_SZ + (TILE_SZ - img_for_draw.Height()) / 2}, true);
    }

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

    GameMap::GameRoom *now_room = nullptr;
    
    int now_x = 0;
    int now_y = 0;
};

#endif