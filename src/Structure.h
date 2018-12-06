#pragma once

#include "ECS/Manager.h"

#include "Components.h"

#include "Config.h"
#include "Graphics.h"

class World;

class Structure {
public:
  const ECS::Entity id;

  constexpr static HealthValue max_health = 500;

  Structure(glm::vec2 pos, World&);

  Structure& operator=(const Structure& o) {
    ECS::Entity& _id = const_cast<ECS::Entity&>(id);
    _id = o.id;
    return *this;
  }

  glm::vec2 pos() const;
  HealthValue health() const;
  float percentHealth() const;
};
