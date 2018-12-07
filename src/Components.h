#pragma once

#include "Config.h"
#include "ECS/Component.h"
#include "Graphics.h"

#include "Path.h"

#include <deque>
#include <functional>

class World;

struct TransformComponent : public ECS::Component {
  World& world;

  glm::vec2 pos;
  float rot;

  static constexpr ECS::ComponentTypeId type = 1;

  TransformComponent(World& world, glm::vec2 pos, float rot) : world(world), pos(pos), rot(rot) {}

  void translate(glm::vec2 displacement);
};

struct SelectableComponent : public ECS::Component {
  bool selected;
  glm::vec2 selectionCentroid;

  static constexpr ECS::ComponentTypeId type = 2;

  SelectableComponent() : selected(false) {}
  SelectableComponent(bool selected) : selected(selected) {}
};

struct MotionComponent : public ECS::Component {
  float movementSpeed = 2.f; // TODO: not hardcoded

  Path::iterator currentTarget; // when path.empty, this iterator is invalid
  glm::vec2 target{0, 0};
  bool hasTarget = false;

  glm::vec2 oldPosition; // where it was before moving to current path waypoint
  Path path;

  static constexpr ECS::ComponentTypeId type = 3;

  MotionComponent() {}

  void pathTo(glm::vec2 pos);
  void repath() {
    if (hasTarget) {
      pathTo(target);
    }
  }
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

  float attackRange;

  ECS::Entity target;
  bool battling;

  float attackTimer;
  float attackCooldown;

  static constexpr ECS::ComponentTypeId type = 6;

  AttackComponent(StrengthValue strength, float attackRange, float attackCooldown)
      : strength(strength), attackRange(attackRange), target(ECS::InvalidEntityId), battling(false),
        attackTimer(attackCooldown), attackCooldown(attackCooldown) {}
};

struct ResourceComponent : public ECS::Component {
  ResourceType speed;

  static constexpr ECS::ComponentTypeId type = 7;

  ResourceComponent(ResourceType accumulationSpeed)
      : speed(accumulationSpeed) {}
};
