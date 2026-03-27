#include <raylib.h>
#include <raymath.h>
#include "Player.hpp"
#include "Level.hpp"
#include "Enemy.hpp" 
#include <iostream>

/* player methods */

void Player::Update(float delta_time) {
  current_state->Update(delta_time);
}

void Player::Draw() {
  DrawCircleV(position, size/2, color);

    if (current_state == &attacking) {
        DrawCircleLines(position.x, position.y, melee_radius, RED);
    }
}

Player::Player(Vector2 pos, float rad, float spd, int hp, Level* lvl) {
  position = pos;
  size = rad;
  speed = spd;
  health = hp;
  level = lvl;
  idle.player = &*this;
  moving.player = &*this;
  attacking.player = &*this;
  blocking.player = &*this;
  dodging.player = &*this;
  melee_radius = 80;
  SetState(&idle);
}

void Player::OnNotify(Entity* source, std::string event) {
  // insert different reactions to different events here
}

void Player::SetState(PlayerState* state) {
  if (current_state != nullptr) {
    current_state->Exit();
  }
  current_state = state;
  current_state->Enter();
}

PlayerState* Player::GetCurrentState() {
  return current_state;
}

/* player state methods */

/* enter methods */

void PlayerIdle::Enter() {
  player->color = SKYBLUE;
  player->velocity = Vector2Zero();
}

void PlayerMoving::Enter() {
  player->color = GREEN;
}

void PlayerAttacking::Enter() {
  player->color = RED;
  attack_time_left = attack_duration;
  hasHit = false;
}

void PlayerBlocking::Enter() {
  player->color = ORANGE;
}

void PlayerDodging::Enter() {
  player->color = PURPLE;
  dodge_time_left = dodge_duration;
}

/* exit methods */

void PlayerIdle::Exit() {}

void PlayerMoving::Exit() {}

void PlayerAttacking::Exit() {}

void PlayerBlocking::Exit() {}

void PlayerDodging::Exit() {}

/* update methods */

void PlayerIdle::Update(float delta_time) {
  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) ||
    IsKeyDown(KEY_D)) {
    player->SetState(&player->moving);
  }
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    player->SetState(&player->attacking);
  }
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    player->SetState(&player->blocking);
  }
}

void PlayerMoving::Update(float delta_time) {
  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S)) {
    if (IsKeyDown(KEY_W)) {
      player->velocity.y = -player->speed;
    }
    if (IsKeyDown(KEY_S)) {
      player->velocity.y = player->speed;
    }
  }
  else {
    player->velocity.y = 0;
  }
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
    if (IsKeyDown(KEY_A)) {
      player->velocity.x = -player->speed;
    }
    if (IsKeyDown(KEY_D)) {
      player->velocity.x = player->speed;
    }
  }
  else {
    player->velocity.x = 0;
  }

  player->MoveWithCollision(player->velocity, delta_time);
  
  if (Vector2Length(player->velocity) == 0) {
    player->SetState(&player->idle);
  }
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    player->SetState(&player->attacking);
  }
  if (IsKeyPressed(KEY_SPACE)) {
    player->SetState(&player->dodging);
  }
}

void PlayerAttacking::Update(float delta_time) {
  attack_time_left -= delta_time;

  if (!hasHit) {
        for (Entity* e : player->observers) {
            Enemy* enemy = (Enemy*)e;
            if (enemy->health <= 0) continue;

            float dist = Vector2Distance(player->position, enemy->position);
            if (dist < player->melee_radius + enemy->size / 2) {
                int damage = 10;                   
                if (enemy->health < damage) damage = enemy->health;
                enemy->health -= damage;
                hasHit = true; // mark that attack already hit

                std::cout << "Enemy hit! HP left: " << enemy->health << std::endl;

                if (enemy->health <= 0) {
                    std::cout << "Enemy defeated!" << std::endl;
                }

                break;
            }
        }
    }

  if (attack_time_left <= 0) {
    player->SetState(&player->idle);
  }
}

void PlayerBlocking::Update(float delta_time) {
  if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    player->SetState(&player->idle);
  }
}

void PlayerDodging::Update(float delta_time) {
  Vector2 dodgeVel = player->velocity;
  if (player->velocity.y < 0) {
    player->velocity.y = -dodge_speed;
  }
  if (player->velocity.x < 0) {
    player->velocity.x = -dodge_speed;
  }
  if (player->velocity.y > 0) {
    player->velocity.y = dodge_speed;
  }
  if (player->velocity.x > 0) {
    player->velocity.x = dodge_speed;
  }

  player->MoveWithCollision(player->velocity, delta_time);

  dodge_time_left -= delta_time;
  if (dodge_time_left <= 0) {
    player->SetState(&player->idle);
  }
}

void Player::MoveWithCollision(Vector2 vel, float delta_time) {
    Vector2 nextPosX = position;
    nextPosX.x += vel.x * delta_time;
    if (!level->CheckCollision(nextPosX, size)) {
        position.x = nextPosX.x;
    }

    Vector2 nextPosY = position;
    nextPosY.y += vel.y * delta_time;
    if (!level->CheckCollision(nextPosY, size)) {
        position.y = nextPosY.y;
    }
}