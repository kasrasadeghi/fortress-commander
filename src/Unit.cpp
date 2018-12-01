#include "Unit.h"
#include "Game.h"

#include "World.h"
#include "Path.h"

Unit::Unit(glm::vec2 pos, World& world) : _target(pos), id(ECS::Manager::createEntity()) {

  ECS::Entity idCopy = id; // have to do this because this pointer gets moved around

  ECS::Manager::addComponent<TransformComponent>(id, TransformComponent(world, pos, 0.f));
  ECS::Manager::addComponent<MotionComponent>(id, MotionComponent());
  ECS::Manager::addComponent<HealthComponent>(id, HealthComponent(health));
  ECS::Manager::addComponent<AttackComponent>(id, AttackComponent(strength, attackCooldown));

  ECS::Manager::addComponent<SelectableComponent>(id, SelectableComponent());
  ECS::Manager::addComponent<CommandableComponent>(
      id, CommandableComponent([idCopy](glm::vec2 pos) {
        ECS::Manager::getComponent<MotionComponent>(idCopy).pathTo(pos);
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
