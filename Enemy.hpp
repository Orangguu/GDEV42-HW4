#ifndef ENEMY
#define ENEMY

#include <raylib.h>
#include <raymath.h>
#include "Entity.hpp"

class Enemy;
class Level;

class EnemyState : public EntityState {
  public:
    Enemy* enemy;
    virtual ~EnemyState() {};
    virtual void Enter() = 0;
    virtual void Update(float delta_time) = 0;
    virtual void Exit() = 0;
};

class EnemyWandering : public EnemyState {
  float direction_duration = 5;
  float direction_time_left = 0;
  public:
    void Enter();
    void Update(float delta_time);
    void Exit();
};

class EnemyChasing : public EnemyState {
  float chasing_speed = 100;
  public:
    void Enter();
    void Update(float delta_time);
    void Exit();
};

class EnemyReadyingAttack : public EnemyState {
  float ready_duration = 3;
  float ready_time_left = 0;
  public:
    void Enter();
    void Update(float delta_time);
    void Exit();
};

class EnemyAttacking : public EnemyState {
  float attack_duration = 0.3;
  float attack_time_left = 0;
  float attack_speed = 900;
  bool hasHit = false;
  public:
    void Enter();
    void Update(float delta_time);
    void Exit();
};

class Enemy : public Entity {
  EnemyState* current_state = nullptr;
  public:
    float rotation;
    Vector2 velocity;
    Vector2 acceleration;
    float speed;
    float detection_radius;
    float aggro_radius;
    float attack_radius;
    Vector2 attack_target;
    EnemyWandering wandering;
    EnemyChasing chasing;
    EnemyReadyingAttack readying_attack;
    EnemyAttacking attacking;
    Entity* player;
    Enemy(Vector2 pos, float size, float spd, int hp, Entity* player, Level* lvl);
    void Update(float delta_time);
    void Draw();
    void OnNotify(Entity* source, std::string event);
    void SetState(EnemyState* state);
    EnemyState* GetCurrentState();
    void MoveWithCollision(Vector2 vel, float delta_time);
};

#endif