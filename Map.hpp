#ifndef MAP
#define MAP

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <queue>
#include "Room.hpp"

class Map;

class Map {
  public:
    int map_width; // in cells
    int map_height; // in cells
    int min_rooms = 10; // in cells
    int max_rooms = 20; // in cells
    float cell_width = 1280; // in pixels
    float cell_height = 768; // in pixels
    std::queue<Room*> checkable_rooms; // all rooms that you can check
    std::vector<Room*> all_rooms;
    std::vector<std::vector<RoomType>> map;
    Map(int width, int height);
    bool IsCellValidRoom(int x, int y);
    void CreateRoomInMap(Room* room);
    void GenerateStartRoom();
    void GenerateBetweenRooms();
    void GenerateEndRooms();
    void GenerateBossRoom();
    void SetUpRooms();
    void DrawMap();
    void DrawGrid();
};

#endif