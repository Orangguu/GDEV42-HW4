#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <queue>
#include <string>
#include "Map.hpp"
#include "Room.hpp"

int dir[4][2] {
    {1, 0}, // RIGHT (0)
    {-1, 0}, // LEFT (1)
    {0, 1}, // DOWN (3)
    {0, -1} // UP (4)
};

Map::Map(int width, int height) {
  this->map_width = width;
  this->map_height = height;
}

bool Map::IsCellValidRoom(int x, int y) {
  if (x < 0 || x >= map_width || y < 0 || y >= map_height)
    return false;

  if (map[x][y] != EMPTY)
      return false;

  int neighbors_count = 0;

  for (int i = 0; i < 4; i++) {
    int new_x = x + dir[i][0];
    int new_y = y + dir[i][1];

    if (new_x < 0 || new_x >= map_width || new_y < 0 || new_y >= map_height)
      continue;

    if (map[new_x][new_y] != EMPTY)
      neighbors_count++;
  }

  return neighbors_count < 2;
}

void Map::CreateRoomInMap(Room* room) {
  int x = room->x;
  int y = room->y;
  RoomType room_type = room->room_type;
  map[x][y] = room_type;
  checkable_rooms.push(room);
  all_rooms.push_back(room);
}

void Map::GenerateStartRoom() {
  int x = GetRandomValue(0, map_width - 1);
  int y = GetRandomValue(0, map_height - 1);

  Room* new_room = new Room(x, y, START);
  CreateRoomInMap(new_room);
}

void Map::GenerateBetweenRooms() {
    int room_count = 1; // INCLUDES THE STARTING ROOM by default

    while(room_count < min_rooms && !checkable_rooms.empty()) {
        Room* current = checkable_rooms.front();
        checkable_rooms.pop();
        int x = current->x;
        int y = current->y;
        // CHECKS FOUR CARDINAL DIRECTIONS (will create rooms there)
        for(int i = 0; i < 4; i++) {
            int new_x = x + dir[i][0];
            int new_y = y + dir[i][1];
            if(IsCellValidRoom(new_x, new_y)) {
                if(GetRandomValue(0,100) < 50) {
                    Room* new_room = new Room(new_x, new_y, REGULAR, current->distance_from_start + 1);
                    switch(i) {
                        case 0: new_room->room_neighbors.right = true;
                        case 1: new_room->room_neighbors.left = true;
                        case 2: new_room->room_neighbors.down = true;
                        case 3: new_room->room_neighbors.up = true;
                    }
                    CreateRoomInMap(new_room);
                    room_count++;
                }
            }
            if(room_count == max_rooms){
                break;
            }
        }
        if(room_count <= max_rooms && checkable_rooms.empty()){ // if there are no more rooms to check
            for(Room* room : all_rooms){
                if(room->room_type == END){ // why would there be end rooms already. double check this
                    room->room_type = REGULAR;
                }
                checkable_rooms.push(room);
            }
        }
    }
}

void Map::GenerateEndRooms() {
    for (Room* room : all_rooms) {
        if (room->room_type == REGULAR) {
            // check for neighbor count (keeping this here since legacy code)
            int neighbors = 0;
            for(int i = 0; i < 4; i++) {
                int nx = room->x + dir[i][0];
                int ny = room->y + dir[i][1];
                if(nx >= 0 && nx < map_width && ny >= 0 && ny < map_height && map[nx][ny] != EMPTY) {
                    neighbors++;
                }
            }
            // if room only has one neighbor, convert to end
            if (neighbors == 1) {
                room->room_type = END;
                map[room->x][room->y] = END;
            }
        }
    }
}

void Map::GenerateBossRoom() {
    // find the end room with the farthest distance (in cells) from the starting room (this will be the boss room)
    if (!all_rooms.empty()) { // make sure that there are rooms that exist first
        Room* farthest_room = nullptr;
        int max_distance = -1;
        for (Room* room : all_rooms) {
            if (room->distance_from_start > max_distance) {
                max_distance = room->distance_from_start;
                farthest_room = room;
            }
        }
        if (farthest_room) {
            farthest_room->room_type = BOSS;
            map[farthest_room->x][farthest_room->y] = BOSS;
        }
    }
}

void Map::SetUpRooms() {
    all_rooms.clear(); // clear rooms
    while(!checkable_rooms.empty()) checkable_rooms.pop(); // clear rooms
    map = std::vector<std::vector<RoomType>>(map_width, std::vector<RoomType>(map_height, EMPTY));
    GenerateStartRoom();
    GenerateBetweenRooms();
    GenerateEndRooms();
    GenerateBossRoom();
}

void Map::DrawMap() {
    for(Room* room : all_rooms){
        std::string distance_str = std::to_string(room->distance_from_start);
        const char* c = distance_str.c_str();
        // Color color = GetRoomColor(room->room_type);
        // room->SetRoomColor();
        // room->SetRoomTexture();
        room->DrawRoom();
        // DrawRectangle(room->x * cell_width, room->y * cell_height, cell_width, cell_height, room->color);
        DrawText(c, room->x *cell_width, room->y * cell_height, 20, WHITE);
    }
}

void Map::DrawGrid() {
    for(int i = 0; i < map_width; i++) {
        for(int j = 0; j < map_height; j++) {
           DrawRectangleLines(i * cell_width, j * cell_height, cell_width, cell_height, WHITE);
        }
    }
};