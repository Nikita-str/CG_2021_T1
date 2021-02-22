#ifndef MAIN_Gmap_H
#define MAIN_Gmap_H

#include <vector>
#include <string>
#include "General.h"


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
    void load(char room_type);

    struct GameRoomInfo // general, lfrom that you need copy data into other class, that changeble it's staitc info!
    {
        GameRoomInfo(char room_type);
        std::vector<std::vector<GameMap::E_TileType>> tile_type{};
        std::vector<std::pair<Point, int>> enemies{}; // point - pos, int - type
        std::vector<Point> door_pos{}; // point - pos
        std::vector< std::pair<Point, int>> items{}; // point - pos,  int - rarity
        std::vector<Point> keys_pos{}; // point - pos
        //enemies: std::vector<>;
        //items: std::vector<>;
    };

    std::vector<std::string> room_map;

    int now_x = 0;
    int now_y = 0;
};

#endif