#pragma once

#include "ECS/Component.h"

struct PositionComponent : public ECS::Component {
  sf::Vector2f _pos;

  static const ECS::ComponentType _type;

  PositionComponent() : _pos(0.f, 0.f) {}
  PositionComponent(sf::Vector2f pos) : _pos(pos) {}
};
const ECS::ComponentType PositionComponent::_type = 1;
