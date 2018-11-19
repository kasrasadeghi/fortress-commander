#pragma once

#include <SFML/Graphics.hpp>

#include "ECS/Component.h"

struct TransformComponent : public ECS::Component {
  sf::Vector2f pos;
  float rot;

  static constexpr ECS::ComponentType _type = 1;

  TransformComponent() : pos(0.f, 0.f), rot(0.f) {}
  TransformComponent(sf::Vector2f pos, float rot) : pos(pos), rot(rot) {}
};
