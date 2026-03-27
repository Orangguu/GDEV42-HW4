#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <raymath.h>
#include "Entity.hpp"

class Player;
class Level;

class PlayerState : public EntityState {
  public:
    Player* player;
    virtual ~PlayerState() {}
    virtual void Enter() = 0;
    virtual void Update(float delta_time) = 0;
    virtual void Exit() = 0;
};

class PlayerIdle : public PlayerState {
  public:
    void Enter();
    void Update(float delta_time);
    void Exit();
};

class PlayerMoving : public PlayerState {
  public:
    void Enter();
    void Update(float delta_time);
    void Exit();
};

class PlayerAttacking: public PlayerState {
  float attack_duration = 1;
  float attack_time_left = 0;
  public:
    bool hasHit = false;
    void Enter();
    void Update(float delta_time);
    void Exit();
};

class PlayerBlocking: public PlayerState {
  public:
    void Enter();
    void Update(float delta_time);
    void Exit();
};

class PlayerDodging: public PlayerState {
  float dodge_duration = 0.2;
  float dodge_time_left = 0;
  float dodge_speed = 1000;
  public:
    void Enter();
    void Update(float delta_time);
    void Exit();
};

class Player : public Entity {
  public:
    PlayerState* current_state = nullptr;
    Vector2 velocity;
    Vector2 acceleration;
    float speed;
    PlayerIdle idle;
    PlayerMoving moving;
    PlayerAttacking attacking;
    PlayerBlocking blocking;
    PlayerDodging dodging;
    float melee_radius;
    Player(Vector2 pos, float rad, float spd, int hp, Level* lvl);
    void Update(float delta_time);
    void Draw();
    void MoveWithCollision(Vector2 vel, float delta_time);
    void OnNotify(Entity* source, std::string event);
    void SetState(PlayerState* state);
    PlayerState* GetCurrentState();
};

#endif