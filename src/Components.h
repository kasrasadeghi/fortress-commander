#pragma once

#include "ECS/Component.h"
#include "Graphics.h"

#include <functional>

struct TransformComponent : public ECS::Component {
  glm::vec2 pos;
  float rot;

  static constexpr ECS::ComponentType _type = 1;

  TransformComponent() : pos(0.f, 0.f), rot(0.f) {}
  TransformComponent(glm::vec2 pos, float rot) : pos(pos), rot(rot) {}
};

struct SelectableComponent : public ECS::Component {
  bool selected;

  static constexpr ECS::ComponentType _type = 2;

  SelectableComponent() : selected(false) {}
  SelectableComponent(bool selected) : selected(selected) {}
};

struct MotionComponent : public ECS::Component {
  float movementSpeed = 2.f; // TODO: not hardcoded
  glm::vec2 velocity;
  glm::vec2 target;
  bool hasTarget = false;

  static constexpr ECS::ComponentType _type = 3;

  MotionComponent() : velocity(0, 0) {}
  MotionComponent(glm::vec2 velocity) : velocity(velocity) {}

  void pathTo(glm::vec2 pos);
};

struct CommandableComponent : public ECS::Component {
  using positionHandlerType = std::function<void(glm::vec2)>;
  positionHandlerType positionHandler;

  static constexpr ECS::ComponentType _type = 4;

  // TODO: support handling a click on another entity (i.e. entityHandler)

  CommandableComponent() : positionHandler([](glm::vec2) {}) {}
  CommandableComponent(positionHandlerType positionHandler) : positionHandler(positionHandler) {}
};