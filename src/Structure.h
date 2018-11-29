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

  static void holo(View& view, glm::vec2 curr) {
    RectangleBatch().add()
      .position(curr)
      .size({tile_size, tile_size})
      .color({0.5, 0.5, 0.5, 0.5})
      .draw(view);
  }
};
