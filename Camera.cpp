#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "Camera.hpp"
#include "Entity.hpp"

GameCamera::GameCamera(Entity* target, float window_width, float window_height) {
  camera_view = new Camera2D();
  camera_view->offset = {window_width/2, window_height/2};
  camera_view->zoom = 1.0f;
  target_entity = target;
}

void GameCamera::Update(float delta_time) {
  camera_view->target = target_entity->position;
}