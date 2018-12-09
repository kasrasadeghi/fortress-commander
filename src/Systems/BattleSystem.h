#pragma once

#include "../Components.h"
#include "../World.h"
#include "../ECS/System.h"
#include "../Events.h"
#include "../GameState.h"

/**
 * @brief Facilitates battle between Units and Enemies
 *
 */
class BattleSystem : public ECS::System {
  void _performAttack(const ECS::Entity entity, const ECS::Entity target) {
    if (entity == ECS::InvalidEntityId || target == ECS::InvalidEntityId) {
      return;
    }

    auto& targetHealth = ECS::Manager::getComponent<HealthComponent>(target).health;
    auto entityStrength = ECS::Manager::getComponent<AttackComponent>(entity).strength;

    // Augment the target's health according to the attack strength
    targetHealth -= entityStrength;

    // Reset the attack timer so we will attack again after attackCooldown
    ECS::Manager::getComponent<AttackComponent>(entity).attackTimer = 0.f;

    glm::vec2 pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
    glm::vec2 tpos = ECS::Manager::getComponent<TransformComponent>(target).pos;
    _gameState._bulletParticles.add(BulletParticle(pos, tpos, 20));
  }

  void _die(const ECS::Entity entity) {
    auto& world = ECS::Manager::getComponent<TransformComponent>(entity).world;

    if (ECS::Manager::hasComponent<CommandableComponent>(entity)) {
      world.removeUnit(entity); // delete unit
    } else {
      world.removeEnemy(entity); // delete enemy
    }
  }

  void _scanForHostiles(const ECS::Entity entity) {
    auto& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
    auto& attack = ECS::Manager::getComponent<AttackComponent>(entity);

    if (ECS::Manager::hasComponent<CommandableComponent>(entity)) {
      for (auto& hostile : _gameState.enemies) {
        auto& hostilePos = ECS::Manager::getComponent<TransformComponent>(hostile.id).pos;
        auto dist = glm::distance(pos, hostilePos);

        if (dist < attack.attackRange) {
          attack.target = hostile.id;
          attack.battling = true;
          return;
        }
      }
    } else {
      for (auto& hostile : _gameState.units) {
        auto& hostilePos = ECS::Manager::getComponent<TransformComponent>(hostile.id).pos;
        auto dist = glm::distance(pos, hostilePos);

        if (dist < attack.attackRange) {
          attack.target = hostile.id;
          return;
        }
      }
    }

    attack.target = ECS::InvalidEntityId;
    attack.battling = false;
    attack.attackTimer = attack.attackCooldown;
  }

public:
  BattleSystem(GameState& gameState) : ECS::System(gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::type);
    requiredComponents.insert(HealthComponent::type);
    requiredComponents.insert(AttackComponent::type);

    setRequiredComponents(std::move(requiredComponents));
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    if (ECS::Manager::getComponent<HealthComponent>(entity).health <= 0) {
      _die(entity);
    }

    auto target = ECS::Manager::getComponent<AttackComponent>(entity).target;

    // Check to see if the target has died
    if (not ECS::Manager::hasEntity(target)) {
      target = ECS::InvalidEntityId;
    }

    if (target == ECS::InvalidEntityId) {
      _scanForHostiles(entity);
      return;
    }

    auto& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
    auto& targetPos = ECS::Manager::getComponent<TransformComponent>(target).pos;

    auto dist = glm::distance(pos, targetPos);

    auto& attack = ECS::Manager::getComponent<AttackComponent>(entity);

    attack.battling = dist < 5.f; // TODO: don't hardcode the attack range, perform raycast?

    if (attack.battling) {
      attack.attackTimer += dt;

      if (attack.attackTimer > attack.attackCooldown) {
        _performAttack(entity, target);
      }
    } else { // Reset the timer so we attack immediately on engaging
      attack.attackTimer = ECS::Manager::getComponent<AttackComponent>(entity).attackCooldown;
    }
  }
};