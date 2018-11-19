#pragma once

#include "ECS/Component.h"

struct TransformComponent : public ECS::Component {
  sf::Vector2f _pos;
  float _rot;

  static const ECS::ComponentType _type;

  TransformComponent() : _pos(0.f, 0.f), _rot(0.f) {}
  TransformComponent(sf::Vector2f pos, float rot) : _pos(pos), _rot(rot) {}
};
const ECS::ComponentType TransformComponent::_type = 1;
