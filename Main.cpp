#include <raylib.h>
#include <vector>
#include <string>
#include <queue>

int windowHeight = 1000;
int windowWidth = 1000;
int map_width = 10;
int map_height = 10;
int min_rooms = 10;
int max_rooms = 20;
float cell_size = 100;

int dir[4][2] {
    {1, 0}, // RIGHT
    {-1, 0}, // LEFT
    {0, 1}, // DOWN
    {0, -1} // UP
};

enum RoomType {
    EMPTY = 0,
    START = 1,
    REGULAR = 2,
    END = 3,
    BOSS = 4,
};

struct Room {
    int x;
    int y;    
    int distance_from_start;
    RoomType room_type;

};

std::queue<Room> checkable_rooms;
std::vector<Room> rooms; // FOR 
std::vector<std::vector<RoomType>> map; // FOR DRAWING

bool isValidRoom(int x, int y) {
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

Color GetRoomColor(RoomType type) {
    switch(type) {
        case START: return SKYBLUE;
        case REGULAR: return GRAY;
        case END: return DARKGRAY;
        case BOSS: return RED;
        default: return BLANK;
    }
}

void CreateRoom(int x, int y, RoomType room_type, int distance = 0) {
    Room room;
    room.x = x;
    room.y = y;
    room.distance_from_start = distance;
    map[x][y] = room_type;
    room.room_type = room_type;
    checkable_rooms.push(room);
    rooms.push_back(room);
}

void GenerateEndRooms() {
    for (Room& room : rooms) {
        if (room.room_type == REGULAR) {
            int neighbors = 0;
            for(int i = 0; i < 4; i++) {
                int nx = room.x + dir[i][0];
                int ny = room.y + dir[i][1];
                if(nx >= 0 && nx < map_width && ny >= 0 && ny < map_height && map[nx][ny] != EMPTY) {
                    neighbors++;
                }
            }
            if (neighbors == 1) {
                room.room_type = END;
                map[room.x][room.y] = END;
            }
        }
    }
}


void GenerateStartRoom() {
    int x = GetRandomValue(0, map_width - 1);
    int y = GetRandomValue(0, map_height - 1);
    CreateRoom(x, y, START);
}

void GenerateRooms() {
    int room_count = 1; // THE STARTING ROOM

    while(room_count < min_rooms && !checkable_rooms.empty()) {
        Room current = checkable_rooms.front();
        checkable_rooms.pop();
        int x = current.x;
        int y = current.y;
        // CHECKS FOUR CARDINAL DIRECTIONS
        for(int i = 0; i < 4; i++) {
            int new_x = x + dir[i][0];
            int new_y = y + dir[i][1];
            if(isValidRoom(new_x, new_y)) {
                if(GetRandomValue(0,100) < 50) {
                    CreateRoom(new_x, new_y, REGULAR, current.distance_from_start + 1);
                    room_count++;
                }
            }
            if(room_count == max_rooms){
                break;
            }
        }
        if(room_count <= max_rooms && checkable_rooms.empty()){
            for(Room& room : rooms){
                if(room.room_type == END){
                    room.room_type = REGULAR;
                }
                checkable_rooms.push(room);
            }
        }
    }
    GenerateEndRooms();

}

void GenerateBossRoom() {
    // Find the room with maximum distance
    if (!rooms.empty()) {
        Room* farthest_room = nullptr;
        int max_distance = -1;
        for (Room& room : rooms) {
            if (room.distance_from_start > max_distance) {
                max_distance = room.distance_from_start;
                farthest_room = &room;
            }
        }
        if (farthest_room) {
            farthest_room->room_type = BOSS;
            map[farthest_room->x][farthest_room->y] = BOSS;
        }
    }
}

void DrawEmptyGrid() {
    for(int i = 0; i < map_width; i++) {
        for(int j = 0; j < map_height; j++) {
           DrawRectangleLines(i * cell_size, j * cell_size, cell_size, cell_size, WHITE);
        }
    }
};

void DrawMap() {
    for(Room room : rooms){
        std::string distance_str = std::to_string(room.distance_from_start);
        const char* c = distance_str.c_str();
        Color color = GetRoomColor(room.room_type);
        DrawRectangle(room.x * cell_size, room.y * cell_size, cell_size, cell_size, color);
        DrawText(c, room.x*cell_size, room.y*cell_size, 20, WHITE);
    }
}

void SetUpRooms() {
    rooms.clear();
    while(!checkable_rooms.empty()) checkable_rooms.pop();
    map = std::vector<std::vector<RoomType>>(map_width, std::vector<RoomType>(map_height, EMPTY));
    GenerateStartRoom();
    GenerateRooms();
    GenerateBossRoom();
}

int main() {
    InitWindow(windowWidth, windowHeight, "Exercise 04 - SimpleDungeonLayoutGeneration");
    map = std::vector<std::vector<RoomType>>(map_width, std::vector<RoomType>(map_height));
    SetUpRooms();

    while(!WindowShouldClose()) {
        if(IsKeyPressed(KEY_R)){
            SetUpRooms();
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawMap();
        DrawEmptyGrid();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
