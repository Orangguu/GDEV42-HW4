#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Level.hpp"

Level::Level(std::ifstream* level_data_file) {
  get_level_from_file(level_data_file);
}

Level::~Level() {
  UnloadTexture(tileset_texture);
}

void Level::get_level_from_file(std::ifstream* level_data_file) {
  std::string line;
  std::string current_category_name = "";
  int current_category_length = 0;
  int current_category_index = 0;

  int i = 0;

  std::cout << "--- Level Data (raw) --- " << std::endl;

  while (getline(*level_data_file, line)) {
    std::cout << line << std::endl;
    std::istringstream iss(line);
    std::string word;
    std::vector<std::vector<int>> grid;
    std::vector<int>* grid_row;


    if (current_category_name == "") {
      iss >> current_category_name;
    }
    if (current_category_name == "PLAYER_POS") {
      iss >> word;
      this->player_position.x = std::stoi(word);
      iss >> word;
      this->player_position.y = std::stoi(word);
      current_category_name = "";
      continue;
    }
    if (current_category_name == "ENEMY_COUNT") {
      iss >> word;
      this->enemy_count = std::stoi(word);
      current_category_name = "ENEMY_DETAILS";
      current_category_length = this->enemy_count;
      current_category_index = i;
      continue;
    }
    if (current_category_name == "ENEMY_DETAILS") {
      Vector2 enemy_position = {};
      iss >> word;
      enemy_position.x = std::stof(word);
      iss >> word;
      enemy_position.y = std::stof(word);
      this->enemy_positions.push_back(enemy_position);
      current_category_length -= 1;
      if (current_category_length <= 0) {
        current_category_name = "";
        current_category_index = 0;
        continue;
      }
    }
    if (current_category_name == "IMAGE_NAME") {
      iss >> word;
      this->tileset_image = word;
      current_category_name = "";
      continue;
    }
    if (current_category_name == "TILE_SIZE") {
      iss >> word;
      this->tile_size.x = std::stoi(word);
      iss >> word;
      this->tile_size.y = std::stoi(word);
      current_category_name = "";
      continue;
    }
    if (current_category_name == "TILE_COUNT") {
      iss >> word;
      this->tile_count = std::stoi(word);
      current_category_name = "TILE_DETAILS";
      current_category_length = this->tile_count;
      current_category_index = i;
      continue;
    }
    if (current_category_name == "TILE_DETAILS") {
      Rectangle tile_rect = {};
      iss >> word;
      tile_rect.x = std::stof(word);
      iss >> word;
      tile_rect.y = std::stof(word);
      iss >> word;
      tile_rect.width = std::stof(word);
      iss >> word;
      tile_rect.height = std::stof(word);
      Tile* tile = new Tile(tile_rect);
      iss >> word;
      if (word == "C") {
        tile->has_collision = true;
      }
      else {
        tile->has_collision = false;
      }
      this->tile_list.push_back(tile);
      current_category_length -= 1;
      if (current_category_length <= 0) {
        current_category_name = "";
        current_category_index = 0;
        continue;
      }
    }
    if (current_category_name == "GRID") {
      iss >> word;
      this->grid_size.x = std::stoi(word);
      iss >> word;
      this->grid_size.y = std::stoi(word);
      current_category_name = "GRID_DETAILS";
      current_category_length = this->grid_size.y;
      current_category_index = i;
      continue;
    }
    if (current_category_name == "GRID_DETAILS") {
      std::vector<int> grid_row;
      while (iss >> word) {
        grid_row.push_back(std::stoi(word));
      }
      this->grid.push_back(grid_row);
      current_category_length -= 1;
      if (current_category_length <= 0) {
        current_category_name = "";
        current_category_index = 0;
        continue;
      }
    }
    i += 1;
  }
  this->tileset_texture = LoadTexture(this->tileset_image.c_str());
}

void Level::print_level_data() {
  std::cout << "--- Level Data (parsed) ---" << std::endl;
  std::cout << "Player: " << std::endl;
  std::cout << "X coord: " << this->player_position.x << std::endl;
  std::cout << "Y coord: " << this->player_position.y << std::endl;
  std::cout << "Enemy count: " << this->enemy_count << std::endl;
  for (int i = 0; i < this->enemy_count; i++) {
    std::cout << "Enemy " << i << ":" << std::endl;
    std::cout << "X coord: " << this->enemy_positions.at(i).x << std::endl;
    std::cout << "Y coord: " << this->enemy_positions.at(i).y << std::endl;
  }
  std::cout << "Tile image: " << this->tileset_image << std::endl;
  std::cout << "Tile size: " << std::endl;
  std::cout << "Width: " << this->tile_size.x << std::endl;
  std::cout << "Height: " << this->tile_size.y << std::endl;
  std::cout << "Tile count: " << this->tile_count << std::endl;
  for (int i = 0; i < this->tile_list.size(); i++) {
    std::cout << "Tile " << i << ":" << std::endl;
    std::cout << "X coord in sprite sheet: " << this->tile_list.at(i)->location_in_sprite_sheet.x << std::endl;
    std::cout << "Y coord in sprite sheet: " << this->tile_list.at(i)->location_in_sprite_sheet.y << std::endl;
    std::cout << "Width in sprite sheet: " << this->tile_list.at(i)->location_in_sprite_sheet.width << std::endl;
    std::cout << "Height in sprite sheet: " << this->tile_list.at(i)->location_in_sprite_sheet.height << std::endl;
    std::cout << "Has collision: " << this->tile_list.at(i)->has_collision << std::endl;
  }
  std::cout << "Grid size: " << std::endl;
  std::cout << "Width: " << this->grid_size.x << std::endl;
  std::cout << "Height: " << this->grid_size.y << std::endl;
  std::cout << "Grid:" << std::endl;
  for (int i = 0; i < this->grid_size.y; i++) {
    std::cout << "Row " << i + 1 << ":" << std::endl;
    for (auto& tile : this->grid.at(i)) {
      std::cout << tile << std::endl;
    }    
  }
}

void Level::Draw() {
    ClearBackground((Color){15, 15, 38, 255}); // hardcoded to match tilemap bg color
    Vector2 current_tile_coords = {0, 0};
    for (int i = 0; i < this->grid_size.y; i++) {
      for (int j = 0; j < this->grid_size.x; j++) {
        int current_tile = (this->grid.at(i).at(j)) - 1; // starting index of grid should be 1
        Rectangle current_tile_source = this->tile_list.at(current_tile)->location_in_sprite_sheet;
        Rectangle current_tile_dest =  {current_tile_coords.x, current_tile_coords.y, this->tile_size.x, this->tile_size.y};
        
        DrawTexturePro(tileset_texture, current_tile_source, current_tile_dest, {0, 0}, 0, WHITE);
        current_tile_coords.x += this->tile_size.x;
      }
      current_tile_coords.x = 0;
      current_tile_coords.y += this->tile_size.y;
    }
}

bool Level::CheckCollision(Vector2 pos, float size) {
  float half = size / 2;

  Vector2 corners[4] = {
    {pos.x - half, pos.y - half},
    {pos.x + half, pos.y - half},
    {pos.x - half, pos.y + half},
    {pos.x + half, pos.y + half}
  };

  for (int i = 0; i < 4; i++) {
    int tileX = corners[i].x / tile_size.x;
    int tileY = corners[i].y / tile_size.y;

    if (tileX < 0 || tileX >= grid_size.x || tileY < 0 || tileY >= grid_size.y)
      return true;
    
    int tileIndex = grid[tileY][tileX] - 1;

    if (tileIndex >= 0 && tileIndex < tile_list.size()) {
      if (tile_list[tileIndex]->has_collision)
        return true;
    }
  }

  return false;
}