#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <fstream>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Camera.hpp"
#include "Level.hpp"
#include "Map.hpp"

const float WINDOW_WIDTH = 1280.0f;
const float WINDOW_HEIGHT = 720.0f;
const int MAP_WIDTH = 10; // in cells/rooms
const int MAP_HEIGHT = 10; // in cells/rooms
const int MIN_ROOMS = 10;
const int MAX_ROOMS = 20;
const float ROOM_WIDTH = WINDOW_WIDTH;
const float ROOM_HEIGHT = WINDOW_HEIGHT;

int main() {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Ongkiko_Reyes_Warain_Homework02");
  SetTargetFPS(60);

  // create level (level contains info about tileset)
  // level creates tiles

  std::ifstream tileset_data_file("tileset.txt");

  if (!tileset_data_file.is_open()) {
    std::cerr << "Error opening the file!";
    return 1;
  }

  Level* level = new Level(&tileset_data_file);

  tileset_data_file.close();
  level->print_level_data();

  // create map (map contains info about rooms)
  // map creates rooms

  Map* map = new Map(MAP_WIDTH, MAP_HEIGHT);
  map->level_info = level;
  map->SetUpRooms();

  // create player
  // get player position from map starting room

  Vector2 player_position = map->player_position;
  float player_size = 40;
  float player_speed = 300;
  int player_health = 50;

  Player* player = new Player(player_position, player_size, player_speed, player_health, level);

  // create enemy
  // get enemy position from map boss room

  Vector2 enemy_position = map->enemy_position;
  float enemy_size = 80;
  float enemy_speed = 50;
  float enemy_health = 20;

  Enemy* enemy = new Enemy(enemy_position, enemy_size, enemy_speed, enemy_health, player, level);

  // create camera
  // use player as the pivot point
  
  GameCamera* camera = new GameCamera(player, WINDOW_WIDTH, WINDOW_HEIGHT);

  while(!WindowShouldClose()) {
    float delta_time = GetFrameTime();
    player->Update(delta_time);
    enemy->Update(delta_time);
    if (enemy->health <= 0) {
      delete enemy;
    }
    camera->Update(delta_time);

    BeginDrawing();
    BeginMode2D(*(camera->camera_view));
    map->DrawMap();
    map->DrawGrid();
    player->Draw();
    enemy->Draw();
    // add HP text for player
    std::string player_hp_text = "Player HP: " + std::to_string(player->health);
    DrawText(player_hp_text.c_str(), 20, 20, 20, GREEN);
    // add HP text for enemy
    std::string enemy_hp_text = "HP: " + std::to_string(enemy->health);
    DrawText(enemy_hp_text.c_str(), enemy->position.x - 20, enemy->position.y - enemy->size/2 - 20, 15, RED);
    EndMode2D();
    EndDrawing();
  }

  CloseWindow();

  delete player;
  delete enemy;
  delete level;
  delete camera;

  return 0;
}