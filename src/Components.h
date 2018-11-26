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

struct SelectableComponent : public ECS::Component {
  bool selected;

  static constexpr ECS::ComponentType _type = 2;

  SelectableComponent() : selected(false) {}
  SelectableComponent(bool selected) : selected(selected) {}
};

struct MotionComponent : public ECS::Component {
  float movementSpeed = 2.f; // TODO: not hardcoded
  sf::Vector2f velocity;
  sf::Vector2f target;
  bool hasTarget = false;

  static constexpr ECS::ComponentType _type = 3;

  MotionComponent() : velocity(0, 0) {}
  MotionComponent(sf::Vector2f velocity) : velocity(velocity) {}

  void pathTo(sf::Vector2f pos);
};