#pragma once

#include "Config.h"
#include "Graphics.h"

class Unit {
  static uint ID_counter;
  glm::vec2 _pos;
  glm::vec2 _target;

public:
  constexpr static float unit_size = 0.5f * tile_size;
  constexpr static float unit_speed = 2.f;
  
  uint ID = ID_counter++; // TODO: unit layout system that when units die you can reuse their ID

  Unit(glm::vec2 pos) : _pos(pos), _target(pos) {}

  glm::vec2 pos() const {
    return _pos;
  }

  static void holo(View& view, glm::vec2 curr) {
    InstancedCircle c(curr.x, curr.y);
    c.size(tile_size, tile_size);
    c.color(1, 0.5, 0.5 /*TODO , 0.5 */);
    c.draw(view);
  }

  void pathTo(glm::vec2 coords);
  void update(float dt);
};
