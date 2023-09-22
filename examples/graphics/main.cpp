#include <iostream>

#include "raylib.h"
#include "emp/math/Random.hpp"
#include "emp/base/vector.hpp"

int main(){
  // Initialize the game window
  const size_t screen_width = 800;
  const size_t screen_height = 800;
  InitWindow(screen_width, screen_height, "Test");
  SetTargetFPS(60);

  emp::Random rng;
  emp::vector<Vector2> box_pos_vec;
  for(size_t i = 0; i < 100ull; ++i){
    box_pos_vec.emplace_back(rng.GetUInt(screen_width), rng.GetUInt(screen_height));
  }
  Vector2 box_size{10, 10};


  bool is_game_running = true;
  while(is_game_running){
    if(IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_Q)){
      is_game_running = false;
    }
    if(IsKeyDown(KEY_SPACE)){
      for(auto & pos : box_pos_vec){
        pos.x = rng.GetUInt(screen_width);
        pos.y = rng.GetUInt(screen_height);
      }
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for(auto pos : box_pos_vec){
      DrawRectangleV(pos, box_size, RED);
    }

    EndDrawing();
  }
  return 0;
}
