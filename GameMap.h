#ifndef MAIN_Gmap_H
#define MAIN_Gmap_H

#include<vector>

enum class E_TileType
{
    Floor,
    Wall,
    Empty
};

struct GameMap
{
    GameMap();

private:
    std::vector<std::vector<E_TileType>> now_wall;
};

#endif