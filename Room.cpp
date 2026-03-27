#include <raylib.h>
#include <raymath.h>
#include <string>
#include "Room.hpp"

Room::Room(int x, int y, RoomType room_type) {
  this->x = x;
  this->y = y;
  this->room_type = room_type;
  this->distance_from_start = 0;
}

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

void Room::SetRoomTexture() {
  if (room_neighbors.up && room_neighbors.down && room_neighbors.left && room_neighbors.right) {
    room_texture = "4dir.txt";
  }
  else if (room_neighbors.up && room_neighbors.down && room_neighbors.left) {
    room_texture = "3dir-left.txt";
  }
  else if (room_neighbors.up && room_neighbors.down && room_neighbors.right) {
    room_texture = "3dir-right.txt";
  }
  else if (room_neighbors.left && room_neighbors.right && room_neighbors.up) {
    room_texture = "3dir-up.txt";
  }
  else if (room_neighbors.left && room_neighbors.right && room_neighbors.down) {
    room_texture = "3dir-down.txt";
  }
  else if (room_neighbors.left && room_neighbors.right) {
    room_texture = "2dir-horizontal.txt";
  }
  else if (room_neighbors.up && room_neighbors.down) {
    room_texture = "2dir-vertical.txt";
  }
  else if (room_neighbors.up && room_neighbors.left) {
    room_texture = "2dir-up+left.txt";
  }
  else if (room_neighbors.up && room_neighbors.right) {
    room_texture = "2dir-up+right.txt";
  }
  else if (room_neighbors.down && room_neighbors.left) {
    room_texture = "2dir-down+left.txt";
  }
  else if (room_neighbors.down && room_neighbors.right) {
    room_texture = "2dir-down+right.txt";
  }
  else if (room_neighbors.up) {
    room_texture = "1dir-up.txt";
  }
  else if (room_neighbors.down) {
    room_texture = "1dir-down.txt";
  }
  else if (room_neighbors.left) {
    room_texture = "1dir-left.txt";
  }
  else if (room_neighbors.right) {
    room_texture = "1dir-right.txt";
  }
  else {
    // room is empty
  }
}