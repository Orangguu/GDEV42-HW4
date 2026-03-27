#ifndef GAMECAMERA
#define GAMECAMERA

#include <raylib.h>
#include <raymath.h>
#include "Entity.hpp"

class GameCamera {
  public:
    Camera2D* camera_view;
    Entity* target_entity;
    GameCamera(Entity* target, float window_width, float window_height);
    void Update(float delta_time);
};

#endif