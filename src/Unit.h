#pragma once

#include "ECS/Manager.h"

#include "Config.h"
#include "Graphics.h"

class World;

class Unit {
protected:
  glm::vec2 _target;

  const ECS::Entity _id;

public:
  constexpr static float unit_size = 0.5f * tile_size;
  constexpr static float unit_speed = 2.f;

  Unit(glm::vec2 pos, World&);

  glm::vec2 pos() const;
  bool selected() const;
  std::vector<glm::ivec2>& path() const;
  glm::ivec2 currentTarget() const;

  static void holo(View& view, glm::vec2 curr) {
    InstancedCircle c(curr.x, curr.y);
    c.size(tile_size, tile_size);
    c.color(1, 0.5, 0.5, 0.5);
    c.draw(view);
  }
};
