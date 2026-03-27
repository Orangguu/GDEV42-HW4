#ifndef LEVEL
#define LEVEL

#include <raylib.h>
#include <raymath.h>
#include <fstream>
#include <string>
#include <vector>
#include "Tile.hpp"

class Level;

class Level {
  public:
    std::string tileset_image;
    Vector2 tile_size;
    int tile_count;
    std::vector<Tile*> tile_list;
    Vector2 grid_size;
    std::vector<std::vector<int>> grid;
    Texture2D tileset_texture;
    Vector2 player_position;
    int enemy_count;
    std::vector<Vector2> enemy_positions;
    Level(std::ifstream* level_data_file);
    ~Level();
    void get_level_from_file(std::ifstream* level_data_file);
    void print_level_data();
    void Update(float delta_time);
    void Draw();
    bool CheckCollision(Vector2 pos, float size);
};

#endif