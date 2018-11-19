#pragma once

#include "ECS/Component.h"

struct TransformComponent : public ECS::Component {
  sf::Vector2f pos;
  float rot;

  static const ECS::ComponentType _type;

  TransformComponent() : pos(0.f, 0.f), rot(0.f) {}
  TransformComponent(sf::Vector2f pos, float rot) : pos(pos), rot(rot) {}
};
const ECS::ComponentType TransformComponent::_type = 1;
