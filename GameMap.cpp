#include "GameMap.h"
#include <fstream>

GameMap *GameMap::cur_map;

const std::string MAP_PATH {"../maps/"};

GameMap::GameMap() : floor_img("../resources/floor", 6), empty_img("../resources/empty.png"), 
                     wall_img("../resources/wall.png"), key_E_img("../resources/E.png", 2, 400)
{
    cur_map = this;

    std::ifstream file {MAP_PATH + "M_OF_M.txt"};

    std::string line;
    int y = 0;
    while (std::getline(file, line)) {
        if (line.size() == 0)continue;
        room_map.push_back(line);
        for (int x = 0; x < line.size(); x++)if (line[x] == 'F') { now_x = x; now_y = y; }
        y++;
    }
    
    file.close();

    load_room(now_x, now_y);
}

Point GameMap::GetPos(E_MapPos map_pos, Size obj_sz)
{
    switch (map_pos) {
    case GameMap::E_MapPos::Center: 
        return Point {.x = (PixWidth() - obj_sz.w) / 2, .y = (PixHeight() - obj_sz.h) / 2};

    default: return Point {0,0}; break;
    }
}


bool GameMap::CanThrowItem(Point pos)
{
    auto & v = GetItems();
    pos = {pos.x / TILE_SZ, pos.y / TILE_SZ};
    for (int i = 0; i < v.size(); i++)if (v[i].pos == pos)return false;
    return E_TileType::Floor == now_room->gri.TileType(pos.x, pos.y);
}


void GameMap::load_room(int x, int y)
{
    char key = room_map[y][x];
    if (!loaded_room_info.contains(key))loaded_room_info.insert(std::pair {key, GameRoomInfo{key}});

    if (!loaded_room.contains({x, y})) {
        loaded_room.emplace(Point {x,y}, GameRoom(loaded_room_info.find(key)->second, *this));
    } 

    now_room = &(loaded_room.find(Point {x,y})->second);
}

void GameMap::Draw(Image &canvas)
{
    now_room->room_holst.FastDraw(canvas, now_room->room_holst.Height());

    now_room->map_objects.DrawItems(canvas);
}

GameMap::GameRoom::GameRoom(GameMap::GameRoomInfo &_gri, GameMap &_parent) : gri(_gri), parent(_parent), map_objects(_parent),
                                                                             room_holst(W_WIDTH, W_HEIGHT, 4)
{
    auto get_draw_point = [&](int x, int y) {return Point {.x = x * TILE_SZ, .y = (gri.map_height - 1 - y) * TILE_SZ}; };

    for (int y = 0; y < gri.map_height; y++) {
        map_ids.emplace_back();
        for (int x = 0; x < gri.map_width; x++){
            Point draw_point = get_draw_point(x, y);
            switch (gri.tile_type[y][x]) {
            case E_TileType::Floor:
            {
                int id = parent.floor_img.GetRandImageId((x + y) % 2);
                map_ids[y].push_back(id);
                parent.floor_img.Draw(id, room_holst, draw_point);
                break;
            }
            case E_TileType::Wall:
            {
                map_ids[y].push_back(0);
                parent.wall_img.Draw(room_holst, draw_point);
                break;
            }
            case E_TileType::Empty:
            {
                map_ids[y].push_back(0);
                parent.empty_img.Draw(room_holst, draw_point);
                break;
            }
            default: error("this type not expected ... what?"); break;
            }
        }
        
        for (int x = gri.map_width; x < (W_WIDTH + TILE_SZ - 1) / TILE_SZ; x++)  // fill with empty tile line to end
            parent.empty_img.Draw(room_holst, get_draw_point(x, y));
    }

    for (int y = gri.map_height; y < (W_HEIGHT + TILE_SZ - 1) / TILE_SZ; y++) // fill all line to ...
        for (int x = 0; x < (W_WIDTH + TILE_SZ - 1) / TILE_SZ; x++) // fill with empty tile line to end
            parent.empty_img.Draw(room_holst, {x * TILE_SZ, y * TILE_SZ});

    for (int i = 0; i < gri.keys_pos.size(); i++)map_objects.AddKey(gri.keys_pos[i]);
    for (int i = 0; i < gri.items.size(); i++)map_objects.AddItem(gri.items[i].first, gri.items[i].second);

}


GameMap::GameRoomInfo::GameRoomInfo(char room_type)
{
    std::ifstream file {MAP_PATH + room_type + ".txt"};
    std::string line;

    int E_index = 0;
    int I_index = 0;

    bool z_open = false;
    bool z_already_was_open = false;

    map_width = -1;
    map_height = 0;

    while (std::getline(file, line)) {
        end_trim(line);
        int sz = line.size();
        if (sz == 0 || line[0] == '/')continue;

        if (line[0] == 'z') {
            if (sz == 1)error("wrong room line : z => z[+|-]");
            bool z_plus = line[1] == '+';
            bool z_minus = line[1] == '-';
            if (!z_plus && !z_minus)error("wrong room line : z => z[+|-]");
            if (z_open && z_plus)error("already open : twice 'z+'");
            if (z_plus && z_already_was_open)error("can be only one definition of room scheme in one file");
            if (z_minus && !z_open)error("room scheme was not open, but close!");
            z_open = !z_open;
            z_already_was_open = true;
            continue;
        }

        if (z_open) {
            if (map_width < 0)map_width = sz;
            if (sz != map_width)error("wrong len of room scheme line : " + line);
            tile_type.emplace_back();
            for (int i = 0; i < sz; i++) {
                char c = line[i];
                if (c == '#')tile_type[map_height].push_back(GameMap::E_TileType::Wall);
                else if (c == ' ')tile_type[map_height].push_back(GameMap::E_TileType::Empty);
                else if (c == '.')tile_type[map_height].push_back(GameMap::E_TileType::Floor);
                else if (c == 'O') {
                    tile_type[map_height].push_back(GameMap::E_TileType::Floor);
                    door_pos.emplace_back(Point {.x = i, .y = map_height});
                } 
                else if (c == 'E') {
                    tile_type[map_height].push_back(GameMap::E_TileType::Floor);
                    enemies.emplace_back(std::pair(Point {.x = i, .y = map_height}, 0));
                }
                else if (c == 'I') {
                    tile_type[map_height].push_back(GameMap::E_TileType::Floor);
                    items.emplace_back(std::pair(Point {.x = i, .y = map_height}, 0));
                } 
                else if (c == 'K') {
                    tile_type[map_height].push_back(GameMap::E_TileType::Floor); 
                    keys_pos.emplace_back(Point {.x = i, .y = map_height});
                }
                else error("unknown type of tile '" + line.substr(0, 1) + "'");
            }
            map_height++;
        }
        else if (line[0] == 'E') {
            if (!z_already_was_open)error("types of enmey ('E : ') must stay after map sheme defenition");

            bool was_num = false;
            int now_value = 0;
            for (int ind = 1; ind < sz; ind++) {
                char c = line[ind];
                if ('0' <= c && c <= '9') {
                    now_value = now_value * 10 + (c - '0'); 
                    was_num = true;
                    if(ind != sz - 1) continue;
                }
                if (!was_num)continue;
                if (enemies.size() <= E_index)error("too many enemies, on map they stay less!");
                enemies[E_index++].second = now_value;
                was_num = false;
                now_value = 0;
            }
        } 
        else if (line[0] == 'I') {
            if (!z_already_was_open)error("types of items ('I : ') must stay after map sheme defenition");

            bool was_num = false;
            int now_value = 0;
            for (int ind = 1; ind < sz; ind++) {
                char c = line[ind];
                if ('0' <= c && c <= '9') {
                    now_value = now_value * 10 + (c - '0');
                    was_num = true;
                    if (ind != sz - 1) continue;
                }
                if (!was_num)continue;
                if (items.size() <= I_index)error("too many items, on map they stay less!");
                items[I_index++].second = now_value;
                was_num = false;
                now_value = 0;
            }
        }
        else error("wrong line : " + line);
    }
    file.close();

    for (int i = 0; i < keys_pos.size(); i++)keys_pos[i].y = map_height - 1 - keys_pos[i].y;
    for (int i = 0; i < items.size(); i++)items[i].first.y = map_height - 1 - items[i].first.y;
}
