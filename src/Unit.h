#pragma once

#include "Config.h"
#include "Graphics.h"

class World;

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
    InstancedCircle(curr.x, curr.y)
      .size(tile_size, tile_size)
      .color(1, 0.5, 0.5, 0.5)
      .draw(view);
  }

  void pathTo(glm::vec2 coords);
  void update(float dt, World& world);
};
