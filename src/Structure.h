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

  Structure(glm::vec2 pos, World&);

  glm::vec2 pos() const;
};
