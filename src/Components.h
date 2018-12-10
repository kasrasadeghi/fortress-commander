#pragma once

#include "Config.h"
#include "ECS/Component.h"
#include "ECS/Manager.h"
#include "Graphics.h"

#include "Path.h"

#include <deque>
#include <functional>
#include <assert.h>

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
  float movementSpeed = 5.f; // TODO: not hardcoded

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

  std::vector<ECS::Entity> attackers;

  void removeAttacker(ECS::Entity attacker) {
    attackers.erase(std::remove(attackers.begin(), attackers.end(), attacker), attackers.end());
  }

  static constexpr ECS::ComponentTypeId type = 5;

  HealthComponent() : health(0) {}
  HealthComponent(HealthValue health) : health(health) {}
};

struct AttackComponent : public ECS::Component {
private:
  ECS::Entity _me;
  ECS::Entity _target;
public:
  StrengthValue strength;

  float range;

  float timer;
  const float cooldown;

  void setTarget(ECS::Entity newTarget) { 
    assert(newTarget != ECS::InvalidEntityId);
    resetTarget();
    _target = newTarget;
    ECS::Manager::getComponent<HealthComponent>(_target).attackers.push_back(_me);
  }

  ECS::Entity target() { return _target; }

  bool hasTarget() { return _target != ECS::InvalidEntityId; }

  void resetTarget() {
    if (not hasTarget()) return;
    ECS::Manager::getComponent<HealthComponent>(_target).removeAttacker(_me);
    _target = ECS::InvalidEntityId;
  }

  // returns something if it has killed it
  void attack() {
    assert(hasTarget());
    auto& targetHealth = ECS::Manager::getComponent<HealthComponent>(_target).health;

    // Augment the target's health according to the attack strength
    targetHealth -= strength;

    // start the cooldown
    timer = cooldown;

    if (targetHealth < 0) {
      kill(); 
    }
  }

  void kill() {
    auto& targetAttackers = ECS::Manager::getComponent<HealthComponent>(_target).attackers;

    assert(ECS::Manager::getComponent<HealthComponent>(_target).health <= 0);

    std::cout << "printing targetAttackers {";
    for (auto& attacker : targetAttackers) {
      std::cout << " " << attacker;
    }
    std::cout << " }" << std::endl;
    
    assert(std::find(targetAttackers.begin(), targetAttackers.end(), _me) != targetAttackers.end());
    assert(std::find(targetAttackers.begin(), targetAttackers.end(), 0)   == targetAttackers.end());
    assert(hasTarget());
    
    for (auto& attacker : targetAttackers) {
      std::cout << attacker << std::endl; 
      ECS::Manager::getComponent<AttackComponent>(attacker).resetTarget();
    }

    assert(not hasTarget());

    // enemies can kill either units or structures
    // if (ECS::Manager::hasComponent<CommandableComponent>(entity)) {
    //   ECS::Manager::getComponent<TransformComponent>(entity).world.removeUnit(entity);
    // } else {
    //   ECS::Manager::getComponent<TransformComponent>(entity).world.removeStructure(entity);
    // }
  }

  void tick(float dt) {
    timer -= dt;
    if (timer < 0) {
      timer = 0;
    }
  }

  bool shouldAttack() { return hasTarget() && timer == 0; }

  static constexpr ECS::ComponentTypeId type = 6;

  AttackComponent(ECS::Entity me, StrengthValue strength, float attackRange, float attackCooldown)
      : _me(me), _target(ECS::InvalidEntityId), strength(strength), range(attackRange),
        timer(0), cooldown(attackCooldown) {}
};

struct SpawnableComponent : public ECS::Component {
  float viewRange; // has to be less than max float - 1 because of EnemySystem::updateEntity currDistance

  SpawnableComponent(float viewRange): viewRange(viewRange) {}

  static constexpr ECS::ComponentTypeId type = 7;
};