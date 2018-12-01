#pragma once

#include "ECS/Component.h"
#include "Graphics.h"
#include "Config.h"

#include "Path.h"

#include <functional>
#include <deque>

class World;

struct TransformComponent : public ECS::Component {
  glm::vec2 pos;
  float rot;

  static constexpr ECS::ComponentTypeId type = 1;

  TransformComponent() : TransformComponent({0.f, 0.f}, 0.f) {}
  TransformComponent(glm::vec2 pos, float rot) : pos(pos), rot(rot) {}

  void translate(glm::vec2 displacement); 
};

struct SelectableComponent : public ECS::Component {
  bool selected;

  static constexpr ECS::ComponentTypeId type = 2;

  SelectableComponent() : selected(false) {}
  SelectableComponent(bool selected) : selected(selected) {}
};

struct MotionComponent : public ECS::Component {
  float movementSpeed = 2.f; // TODO: not hardcoded
  World& world;
  
  glm::vec2 target;
  Path::iterator currentTarget; // when path.empty, this iterator is invalid
  bool hasTarget = false;
  Path path;

  static constexpr ECS::ComponentTypeId type = 3;

  MotionComponent(World& world) : world(world) {}

  void pathTo(glm::vec2 pos);
};

struct CommandableComponent : public ECS::Component {
  using positionHandlerType = std::function<void(glm::vec2)>;
  positionHandlerType positionHandler;

  static constexpr ECS::ComponentTypeId type = 4;

  // TODO: support handling a click on another entity (i.e. entityHandler)

  CommandableComponent() : positionHandler([](glm::vec2) {}) {}
  CommandableComponent(positionHandlerType positionHandler) : positionHandler(positionHandler) {}
};

struct HealthComponent : public ECS::Component {
  HealthValue health;

  static constexpr ECS::ComponentTypeId type = 5;

  HealthComponent() : health(0) {}
  HealthComponent(HealthValue health) : health(health) {}
};

struct AttackComponent : public ECS::Component {
  StrengthValue strength;

  ECS::Entity target;
  bool battling;

  float attackTimer;
  float attackCooldown;

  static constexpr ECS::ComponentTypeId type = 6;

  AttackComponent() : AttackComponent(0, 1.f) {}
  AttackComponent(StrengthValue strength, float attackCooldown) : strength(strength),
	target(ECS::InvalidEntityId), battling(false),
	attackTimer(0.f), attackCooldown(attackCooldown) {}
};
