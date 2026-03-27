#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <fstream>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Camera.hpp"
#include "Level.hpp"

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

  std::ifstream level_data_file("level1.txt");

  if (!level_data_file.is_open()) {
    std::cerr << "Error opening the file!";
    return 1;
  }

  Level* level = new Level(&level_data_file);

  level_data_file.close();
  level->print_level_data();

  Vector2 player_position = level->player_position;
  float player_size = 40;
  float player_speed = 300;
  int player_health = 50;
  
  Player* player = new Player(player_position, player_size, player_speed, player_health, level);

  std::vector<Enemy*> enemies;

  for (Vector2 pos : level->enemy_positions) {
    enemies.push_back(new Enemy(pos, 80, 50, 20, player, level));
  }

  for (Enemy* e : enemies) {
    e->AddObserver(player);
    e->AddObserver(e);       // enemy reacts to self events
    player->AddObserver(e);  // player reacts to enemies (for attacking)
  }
  
  GameCamera* camera = new GameCamera(player, WINDOW_WIDTH, WINDOW_HEIGHT);

  while(!WindowShouldClose()) {
    float delta_time = GetFrameTime();
    player->Update(delta_time);
    for (Enemy* e : enemies) {
        e->Update(delta_time);
    }
    for (int i = enemies.size() - 1; i >= 0; i--) {
      if (enemies[i]->health <= 0) {
          delete enemies[i];
          enemies.erase(enemies.begin() + i);
      }
    }
    camera->Update(delta_time);

    BeginDrawing();
    BeginMode2D(*(camera->camera_view));
    level->Draw();
    player->Draw();
    for (Enemy* e : enemies) {
        e->Draw();
    }

    for (Enemy* e : enemies) {
      std::string enemy_hp_text = "HP: " + std::to_string(e->health);
      DrawText(enemy_hp_text.c_str(), e->position.x - 20, e->position.y - e->size/2 - 20, 15, RED);
    }
    
    EndMode2D();

    std::string player_hp_text = "Player HP: " + std::to_string(player->health);
    DrawText(player_hp_text.c_str(), 20, 20, 20, GREEN);


    EndDrawing();
  }

  CloseWindow();
  delete player;
  delete level;
  delete camera;
  for (Enemy* e : enemies)
      delete e;
  enemies.clear();
  return 0;
}