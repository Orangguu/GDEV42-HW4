#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Room.hpp"

Room::Room(int x, int y, RoomType room_type, int distance_from_start) {
  this->x = x;
  this->y = y;
  this->room_type = room_type;
  this->distance_from_start = distance_from_start;
}

void Room::SetRoomColor() {
  switch(room_type) {
    case START: this->room_color = SKYBLUE;
    case REGULAR: this->room_color = GRAY;
    case END: this->room_color = DARKGRAY;
    case BOSS: this->room_color = RED;
    default: this->room_color = BLANK;
  }
}

void Room::SetRoomTilemap() {
  if (room_neighbors.up && room_neighbors.down && room_neighbors.left && room_neighbors.right) {
    room_tilemap_file = "rooms/4dir.txt";
  }
  else if (room_neighbors.up && room_neighbors.down && room_neighbors.left) {
    room_tilemap_file = "rooms/3dir-left.txt";
  }
  else if (room_neighbors.up && room_neighbors.down && room_neighbors.right) {
    room_tilemap_file = "rooms/3dir-right.txt";
  }
  else if (room_neighbors.left && room_neighbors.right && room_neighbors.up) {
    room_tilemap_file = "rooms/3dir-up.txt";
  }
  else if (room_neighbors.left && room_neighbors.right && room_neighbors.down) {
    room_tilemap_file = "rooms/3dir-down.txt";
  }
  else if (room_neighbors.left && room_neighbors.right) {
    room_tilemap_file = "rooms/2dir-horizontal.txt";
  }
  else if (room_neighbors.up && room_neighbors.down) {
    room_tilemap_file = "rooms/2dir-vertical.txt";
  }
  else if (room_neighbors.up && room_neighbors.left) {
    room_tilemap_file = "rooms/2dir-up+left.txt";
  }
  else if (room_neighbors.up && room_neighbors.right) {
    room_tilemap_file = "rooms/2dir-up+right.txt";
  }
  else if (room_neighbors.down && room_neighbors.left) {
    room_tilemap_file = "rooms/2dir-down+left.txt";
  }
  else if (room_neighbors.down && room_neighbors.right) {
    room_tilemap_file = "rooms/2dir-down+right.txt";
  }
  else if (room_neighbors.up) {
    room_tilemap_file = "rooms/1dir-up.txt";
  }
  else if (room_neighbors.down) {
    room_tilemap_file = "rooms/1dir-down.txt";
  }
  else if (room_neighbors.left) {
    room_tilemap_file = "rooms/1dir-left.txt";
  }
  else if (room_neighbors.right) {
    room_tilemap_file = "rooms/1dir-right.txt";
  }
  else {
    room_tilemap_file = "";
  }

  std::ifstream room_tilemap_data(room_tilemap_file);

  if (!room_tilemap_data.is_open()) {
    std::cerr << "Error opening the file!";
  }

  std::string line;
  int i = 0;

  while (getline(room_tilemap_data, line)) {
    std::istringstream iss(line);
    std::string word;
    std::vector<int> tilemap_row;
    while (iss >> word) {
      tilemap_row.push_back(std::stoi(word));
    }
    room_tilemap.push_back(tilemap_row);
  }

  room_tilemap_data.close();
}

void Room::Draw() {
  ClearBackground((Color){15, 15, 38, 255}); // hardcoded to match tilemap bg color
  Vector2 current_tile_coords = {x, y};
  for (int i = 0; i < level_info->tilemap_size.y; i++) {
    for (int j = 0; j < level_info->tilemap_size.x; j++) {
      int current_tile = (room_tilemap.at(i).at(j)) - 1; // starting index of grid should be 1
      Rectangle current_tile_source = level_info->tile_list.at(current_tile)->location_in_sprite_sheet;
      Rectangle current_tile_dest =  {current_tile_coords.x, current_tile_coords.y, level_info->tile_size.x, level_info->tile_size.y};
      DrawTexturePro(level_info->tileset_texture, current_tile_source, current_tile_dest, {0, 0}, 0, WHITE);
      current_tile_coords.x += level_info->tile_size.x;
    }
    current_tile_coords.x = x;
    current_tile_coords.y += level_info->tile_size.y;
  }
}