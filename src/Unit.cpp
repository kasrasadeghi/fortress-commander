#include "Unit.h"
#include "Game.h"

#include "Path.h"
#include "World.h"

Unit::Unit(glm::vec2 pos, World& world) : _target(pos), id(ECS::Manager::createEntity()) {

  ECS::Entity idCopy = id; // have to do this because this pointer gets moved around

  ECS::Manager::addComponent<TransformComponent>(id, TransformComponent(world, pos, 0.f));
  ECS::Manager::addComponent<MotionComponent>(id, MotionComponent());
  ECS::Manager::addComponent<HealthComponent>(id, HealthComponent(max_health));
  ECS::Manager::addComponent<AttackComponent>(id, AttackComponent(strength, radius, attackCooldown));

  ECS::Manager::addComponent<SelectableComponent>(id, SelectableComponent());
  ECS::Manager::addComponent<CommandableComponent>(
      id, CommandableComponent([idCopy](glm::vec2 targetPos) {
        auto& pos = ECS::Manager::getComponent<TransformComponent>(idCopy).pos;
        auto& centroid = ECS::Manager::getComponent<SelectableComponent>(idCopy).selectionCentroid;
        glm::vec2 offset = pos - centroid;
        ECS::Manager::getComponent<MotionComponent>(idCopy).pathTo(targetPos + offset);
      }));
  ECS::Manager::registerEntity(id);
}

glm::vec2 Unit::pos() const {
  return ECS::Manager::getComponent<TransformComponent>(id).pos;
}

bool Unit::selected() const {
  return ECS::Manager::getComponent<SelectableComponent>(id).selected;
}

Path& Unit::path() const {
  return ECS::Manager::getComponent<MotionComponent>(id).path;
}

void Unit::repath() const {
  return ECS::Manager::getComponent<MotionComponent>(id).repath();
}

glm::ivec2 Unit::currentTarget() const {
  return ECS::Manager::getComponent<MotionComponent>(id).currentTarget.operator*();
}

HealthValue Unit::health() const {
  return ECS::Manager::getComponent<HealthComponent>(id).health;
}
