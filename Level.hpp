#ifndef LEVEL
#define LEVEL

#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <string>
#include <vector>
#include "Tile.hpp"

// still checking which variables to remove or move to another file

class Level;

class Level {
  public:
    std::string tileset_image;
    Texture2D tileset_texture;
    Vector2 tile_size;
    int tile_count;
    std::vector<Tile*> tile_list;
    Vector2 tilemap_size;
    // std::vector<std::vector<int>> grid;
    // Vector2 player_position;
    // int enemy_count;
    // Vector2 enemy_position;
    Level(std::ifstream* level_data_file);
    ~Level();
    void get_level_from_file(std::ifstream* level_data_file);
    void print_level_data();
    // void Update(float delta_time);
    // void Draw();
    // bool CheckCollision(Vector2 pos, float size);
};

#endif