#pragma once

#include "ECS/Manager.h"

#include "Components.h"

#include "Config.h"
#include "Graphics.h"
#include "Path.h"

class World;

class Enemy {
protected:
  glm::vec2 _target;

public:
  const ECS::Entity _id;

  constexpr static float unit_size = 0.5f * tile_size;
  constexpr static float unit_speed = 2.f;
  constexpr static HealthValue health = 100;
  constexpr static StrengthValue strength = 25;
  constexpr static float attackCooldown = 1.f;

  Enemy(glm::vec2 pos, World&);

  glm::vec2 pos() const;
  bool selected() const;
  Path& path() const;
  glm::ivec2 currentTarget() const;
  void pathTo(glm::vec2 v);
};
