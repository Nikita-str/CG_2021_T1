#ifndef MAIN_Gmap_H
#define MAIN_Gmap_H

#include <vector>
#include <map>
#include <string>

#include "General.h"
#include "Image.h"
#include "RandImage.h"

static constexpr int TILE_SZ = 32;


struct GameMap
{
    enum class E_TileType
    {
        Floor,
        Wall,
        Empty
    };

    GameMap();

    void Draw(Image &canvas);

//private:
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
    };

    struct GameRoom
    {
        std::vector<std::vector<int>> map_ids{};
        GameMap::GameRoomInfo &gri;
        Image room_holst;
        GameRoom(GameMap::GameRoomInfo &_gri, GameMap &_parent);
    private:
        GameMap &parent;
    };

    std::vector<std::string> room_map {};
    std::map<Point, GameRoom> loaded_room{};
    std::map<char, GameRoomInfo> loaded_room_info {};

    RandImage floor_img;
    Image empty_img;

    GameMap::GameRoom *now_room = nullptr;
    
    int now_x = 0;
    int now_y = 0;
};

#endif