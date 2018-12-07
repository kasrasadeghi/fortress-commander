#pragma once

#include "ECS/Manager.h"

#include "Components.h"

#include "Config.h"
#include "Graphics.h"

class World;

class Structure {
public:
  const ECS::Entity id;

  constexpr static HealthValue health = 500;
  constexpr static ResourceType resourceSpeed = 0.3f;
  constexpr static ResourceType cost = 100.f;

  Structure(glm::vec2 pos, World&);

  Structure& operator=(const Structure& o) {
    ECS::Entity& _id = const_cast<ECS::Entity&>(id);
    _id = o.id;
    return *this;
  }

  glm::vec2 pos() const;
};
