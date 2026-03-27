#ifndef ENTITY
#define ENTITY

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <string>

class Entity;
class Level;

class EntityState {
  public:
    virtual ~EntityState() {}
    virtual void Enter() = 0;
    virtual void Update(float delta_time) = 0;
    virtual void Exit() = 0;
};

class Entity {
  public:
    Vector2 position;
    float size;
    Color color;
    int health;
    Level* level;
    std::vector<Entity*> observers;
    virtual void Update(float delta_time) = 0;
    virtual void Draw() = 0;
    virtual void OnNotify(Entity* source, std::string event) = 0;
    virtual void AddObserver(Entity* observer);
    virtual void RemoveObserver(Entity* observer);
    virtual void Notify(Entity* source, std::string event);
};

#endif