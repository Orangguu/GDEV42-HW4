#include <raylib.h>
#include <raymath.h>
#include <random>
#include <chrono>
#include <cmath>
#include <iostream>
#include "Enemy.hpp"
#include "Player.hpp"
#include "Level.hpp"

void Enemy::Update(float delta_time) {
  current_state->Update(delta_time);
}

void Enemy::Draw() {
  DrawRectanglePro({position.x, position.y, size, size}, {size/2, size/2}, rotation, color);
  DrawCircleLines(position.x, position.y, detection_radius, GREEN);
  DrawCircleLines(position.x, position.y, aggro_radius, YELLOW);
  DrawCircleLines(position.x, position.y, attack_radius, RED);
}

Enemy::Enemy(Vector2 pos, float size, float spd, int hp, Entity* player, Level* lvl) {
  position = pos;
  this->size = size;
  speed = spd;
  health = hp;
  level = lvl;
  wandering.enemy = &*this;
  chasing.enemy = &*this;
  readying_attack.enemy = &*this;
  attacking.enemy = &*this;
  this->player = player;
  detection_radius = 500;
  aggro_radius = 300;
  attack_radius = 150;
  SetState(&wandering);
}

void Enemy::OnNotify(Entity* source, std::string event) {
  // insert different reactions to different events here
  // note: use GetCurrentState() if multiple states use the same events
  if (event == "player_in_detection") {
    SetState(&chasing);
    std::cout << "in detection range, enemy will chase" << std::endl;
  }
  if (event == "player_not_in_detection") {
    SetState(&wandering);
    std::cout << "not in detection range, enemy will stop chasing" << std::endl;
  }
  if (event == "player_in_attack") {
    SetState(&readying_attack);
    std::cout << "player has aggroed enemy, readying attack" << std::endl;
  }
}

void Enemy::SetState(EnemyState* state) {
  if (current_state != nullptr) {
    current_state->Exit();
  }
  current_state = state;
  current_state->Enter();
}

EnemyState* Enemy::GetCurrentState() {
  return current_state;
}

/* enemy state methods */

/* enter methods */

void EnemyWandering::Enter() {
  enemy->color = SKYBLUE;
  enemy->velocity = Vector2Zero();
  enemy->rotation = 0;
}

void EnemyChasing::Enter() {
  enemy->color = YELLOW;
  enemy->velocity = Vector2Zero();
  enemy->rotation = 0;
}

void EnemyReadyingAttack::Enter() {
  enemy->color = ORANGE;
  enemy->velocity = Vector2Zero();
  ready_time_left = ready_duration;
}

void EnemyAttacking::Enter() {
  enemy->color = RED;
  enemy->velocity = Vector2Zero();
  attack_time_left = attack_duration;
}

/* exit methods*/

void EnemyWandering::Exit() {
  enemy->velocity = Vector2Zero();
  direction_time_left = 0;
}

void EnemyChasing::Exit() {}

void EnemyReadyingAttack::Exit() {}

void EnemyAttacking::Exit() {}

/* update methods */

void EnemyWandering::Update(float delta_time) {
  direction_time_left -= delta_time;
  if (direction_time_left <= 0) {
    // change direction
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> distribution(0, 1);
    int random_direction_x = distribution(rng);
    int random_direction_y = distribution(rng);
    if ((random_direction_x == 0) && (random_direction_y == 0)) { // prevent enemy from stopping sometimes
      enemy->velocity.x = enemy->speed * 1;
      enemy->velocity.y = enemy->speed * 1;
    }
    else {
      enemy->velocity.x = enemy->speed * random_direction_x;
      enemy->velocity.y = enemy->speed * random_direction_y;
    }
    if ((enemy->velocity.x == 0) || (enemy->velocity.y == 0)) {
      enemy->rotation = 0;
    }
    else {
      enemy->rotation = 45;
    }
    direction_time_left = direction_duration;
  }
  enemy->MoveWithCollision(enemy->velocity, delta_time);
  float player_distance = Vector2Length(Vector2Subtract(enemy->player->position, enemy->position));
  float radii_sum = enemy->player->size + enemy->aggro_radius;
  if (player_distance <= radii_sum) {
    enemy->Notify(enemy, "player_in_detection");
  }
}

void EnemyChasing::Update(float delta_time) {
  Vector2 starting_vector = {0, -1};  
  Vector2 vector_to_player = Vector2Subtract(enemy->player->position, enemy->position);
  enemy->rotation = acosf(Vector2DotProduct(starting_vector, vector_to_player)/(Vector2Length(starting_vector) * Vector2Length(vector_to_player))) * RAD2DEG;
  if (enemy->player->position.x < enemy->position.x) {
    enemy->rotation = enemy->rotation * -1;
  }
  enemy->velocity = (Vector2){chasing_speed, chasing_speed} * Vector2Normalize(vector_to_player);
  enemy->MoveWithCollision(enemy->velocity, delta_time);
  float player_distance = Vector2Length(Vector2Subtract(enemy->player->position, enemy->position));
  float detection_radii_sum = enemy->player->size + enemy->detection_radius;
  if (player_distance > detection_radii_sum) {
    enemy->Notify(enemy, "player_not_in_detection");
  }
  float attack_radii_sum = enemy->player->size + enemy->attack_radius;
  if (player_distance <= attack_radii_sum) {
    enemy->Notify(enemy, "player_in_attack");
  }
}

void EnemyReadyingAttack::Update(float delta_time) {
  Vector2 starting_vector = {0, -1};  
  Vector2 vector_to_player = Vector2Subtract(enemy->player->position, enemy->position);
  enemy->rotation = acosf(Vector2DotProduct(starting_vector, vector_to_player)/(Vector2Length(starting_vector) * Vector2Length(vector_to_player))) * RAD2DEG;
  if (enemy->player->position.x < enemy->position.x) {
    enemy->rotation = enemy->rotation * -1;
  }
  ready_time_left -= delta_time;
  if (ready_time_left <= 0) {
    enemy->attack_target = vector_to_player;
    enemy->SetState(&enemy->attacking); // not sure if i should make this an event since only enemy needs to know
  }
}

// void EnemyAttacking::Update(float delta_time) {
//   enemy->velocity = (Vector2){attack_speed, attack_speed} * Vector2Normalize(enemy->attack_target);
//   enemy->position += enemy->velocity * delta_time;
//   attack_time_left -= GetFrameTime();
//   if (attack_time_left <= 0) {
//     enemy->SetState(&enemy->wandering);
//   }
// }

void EnemyAttacking::Update(float delta_time) {
    enemy->velocity = (Vector2){attack_speed, attack_speed} * Vector2Normalize(enemy->attack_target);
    enemy->MoveWithCollision(enemy->velocity, delta_time);

    Player* p = (Player*)enemy->player;
    float dist_to_player = Vector2Distance(enemy->position, p->position);

    if (!hasHit && dist_to_player < enemy->size/2 + p->size/2) {
        int damage = 10;
        if (p->current_state == &p->blocking) damage /= 2;
        else if (p->current_state == &p->dodging) damage = 0;

        p->health -= damage;
        hasHit = true;
        std::cout << "Player hit! HP left: " << p->health << std::endl;
    }

    attack_time_left -= delta_time;

    if (attack_time_left <= 0) {
        enemy->SetState(&enemy->wandering);
        hasHit = false;
    }
}

void Enemy::MoveWithCollision(Vector2 vel, float delta_time) {
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