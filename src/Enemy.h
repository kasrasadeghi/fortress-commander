#pragma once

#include "ECS/Manager.h"

#include "Components.h"

#include "Config.h"
#include "Graphics.h"

class World;

class Enemy {
protected:
  glm::vec2 _target;

  const ECS::Entity _id;

public:
  constexpr static float unit_size = 0.5f * tile_size;
  constexpr static float unit_speed = 2.f;
  constexpr static HealthComponent::HealthValue health = 100;
  constexpr static AttackComponent::StrengthValue strength = 25;

  Enemy(glm::vec2 pos, World&);

  glm::vec2 pos() const;
  bool selected() const;
  std::vector<glm::ivec2>& path() const;
  glm::ivec2 currentTarget() const;
  void pathTo(glm::vec2 v);
};
