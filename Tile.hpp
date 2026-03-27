#ifndef TILE
#define TILE

#include <raylib.h>
#include <raymath.h>

class Tile {
  public:
    Rectangle location_in_sprite_sheet;
    Tile(Rectangle location) {
      location_in_sprite_sheet = location;
    }
    bool has_collision;
};

#endif