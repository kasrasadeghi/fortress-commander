#pragma once

#include "Config.h"
#include "ECS/Component.h"
#include "ECS/Manager.h"
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

  static constexpr ECS::ComponentTypeId type = 2;

  SelectableComponent() : selected(false) {}
  SelectableComponent(bool selected) : selected(selected) {}
};

struct MotionComponent : public ECS::Component {
  float movementSpeed = 2.f; // TODO: not hardcoded

  Path::iterator currentTarget; // when path.empty, this iterator is invalid
  glm::vec2 target{0, 0};
  bool hasTarget = false;
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

  float range;

  ECS::Entity target;

  float timer;
  const float cooldown;

  void setTarget(ECS::Entity target) { 
    this->target = target; 
  }

  bool hasTarget() { return target != ECS::InvalidEntityId; }

  void resetTarget() {
    target = ECS::InvalidEntityId;
  }

  // returns something if it has killed it
  ECS::Entity attack() {
    auto& targetHealth = ECS::Manager::getComponent<HealthComponent>(target).health;

    // Augment the target's health according to the attack strength
    targetHealth -= strength;

    // start the cooldown
    timer = cooldown;

    if (targetHealth < 0) {
      auto result = kill();
      if (result) return result;
    }
    return ECS::InvalidEntityId;
  }

  ECS::Entity kill() {
    auto t = target;
    resetTarget();
    return t;
  }

  void tick(float dt) {
    timer -= dt;
    if (timer < 0) {
      timer = 0;
    }
  }

  bool shouldAttack() { return hasTarget() && timer == 0; }

  static constexpr ECS::ComponentTypeId type = 6;

  AttackComponent(StrengthValue strength, float attackRange, float attackCooldown)
      : strength(strength), range(attackRange), target(ECS::InvalidEntityId),
        timer(0), cooldown(attackCooldown) {}
};

struct SpawnableComponent : public ECS::Component {
  float viewRange; // has to be less than max float - 1 because of EnemySystem::updateEntity currDistance

  SpawnableComponent(float viewRange): viewRange(viewRange) {}

  static constexpr ECS::ComponentTypeId type = 7;
};