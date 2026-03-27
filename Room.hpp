#ifndef ROOM
#define ROOM

#include <raylib.h>
#include <raymath.h>
#include <string>

enum RoomType {
  EMPTY = 0,
  START = 1,
  REGULAR = 2,
  END = 3,
  BOSS = 4,
};

struct RoomNeighbors {
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
};

struct RoomTextures {
  std::string four_way = "4dir.txt";
  std::string three_way_up = "3dir-up.txt";
  std::string three_way_down = "3dir-down.txt";
  std::string three_way_left = "3dir-left.txt";
  std::string three_way_down = "3dir-right.txt";
  std::string two_way_horizontal = "2dir-horizontal.txt";
  std::string two_way_vertical = "2dir-vertical.txt";
  std::string two_way_up_left = "2dir-up+left.txt";
  std::string two_way_up_right = "2dir-up+right.txt";
  std::string two_way_down_left = "2dir-down+left.txt";
  std::string two_way_down_right = "2dir-down+right.txt";
  std::string one_way_up = "1dir-up.txt";
  std::string one_way_down = "1dir-down.txt";
  std::string one_way_left = "1dir-left.txt";
  std::string one_way_right = "1dir-right.txt";
};

class Room {
  public:
    int x; // in cells
    int y; // in cells
    int distance_from_start; // in cells
    RoomType room_type;
    RoomNeighbors room_neighbors;
    Color room_color;
    std::string room_texture;
    Room(int x, int y, RoomType room_type);
    Room(int x, int y, RoomType room_type, int distance_from_start);
    void SetRoomColor();
    void SetRoomTexture();
    void DrawRoom();
};

#endif