#pragma once

#include "ECS/Manager.h"
#include "Components.h"

#include "Config.h"
#include "Graphics.h"

class Unit {
  glm::vec2 _target;

  ECS::Manager& _manager;
  const ECS::Entity _id;


public:
  constexpr static float unit_size = 0.5f * tile_size;
  constexpr static float unit_speed = 2.f;

  Unit(glm::vec2 pos, ECS::Manager& manager);

  glm::vec2 pos() const {
    return _manager.getComponent<TransformComponent>(_id).pos;
  }

  static void holo(View& view, glm::vec2 curr) {
    InstancedCircle c(curr.x, curr.y);
    c.size(tile_size, tile_size);
    c.color(1, 0.5, 0.5, 0.5);
    c.draw(view);
  }

  void pathTo(glm::vec2 coords);
  void update(float dt);
};
