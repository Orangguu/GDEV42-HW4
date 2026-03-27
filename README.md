# GDEV 42 - Homework 3

Compilation command (Assuming that the `include/` and `lib/` folders are in the root project directory):

```bash
g++ Main.cpp Entity.cpp Player.cpp Enemy.cpp Camera.cpp Level.cpp Tile.cpp Map.cpp Room.cpp -o Main -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
```