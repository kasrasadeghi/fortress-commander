#pragma once

#include "ECS/Manager.h"

#include "Components.h"

#include "Config.h"
#include "Graphics.h"

class World;

class Structure {
  glm::vec2 _target;

public:
  const ECS::Entity _id;

  constexpr static HealthValue health = 500;

  Structure(glm::vec2 pos, World&);

  glm::vec2 pos() const;

  static void holo(View& view, glm::ivec2 p) {
    glm::vec2 offset(-0.5, -0.5);

    RectangleBatch().add()
      .position(glm::vec2(p.x * tile_size, p.y * tile_size) - offset * tile_size)
      .color({.7, .7, .7, .5})
      .size({tile_size, tile_size})
      .draw(view);
  }
};
