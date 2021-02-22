#ifndef MAIN_Gmap_H
#define MAIN_Gmap_H

#include <vector>
#include <map>
#include <string>

#include "General.h"
#include "Image.h"

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
    };

    struct GameRoom
    {
        GameMap::GameRoomInfo &gri;
        Image room_holst;
        GameRoom(GameMap::GameRoomInfo &_gri);
    };

    std::vector<std::string> room_map;
    std::vector<std::vector<GameRoom>> loaded_room{};
    std::map<char, GameRoomInfo> loaded_room_info {};


    
    int now_x = 0;
    int now_y = 0;
};

#endif