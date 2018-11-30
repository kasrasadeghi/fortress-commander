#include "Unit.h"
#include "Game.h"

#include "World.h"
#include "Path.h"

Unit::Unit(glm::vec2 pos, World& world) : _target(pos), _id(ECS::Manager::createEntity()) {

  ECS::Entity id = _id; // have to do this because this pointer gets moved around

  ECS::Manager::addComponent<TransformComponent>(_id, TransformComponent(pos, 0.f));
  ECS::Manager::addComponent<MotionComponent>(_id, MotionComponent(world));
  ECS::Manager::addComponent<HealthComponent>(_id, HealthComponent(health));
  ECS::Manager::addComponent<AttackComponent>(_id, AttackComponent(strength, attackCooldown));

  ECS::Manager::addComponent<SelectableComponent>(_id, SelectableComponent());
  ECS::Manager::addComponent<CommandableComponent>(
      _id, CommandableComponent([id](glm::vec2 pos) {
        ECS::Manager::getComponent<MotionComponent>(id).pathTo(pos);
      }));
  ECS::Manager::registerEntity(_id);
}

glm::vec2 Unit::pos() const {
  return ECS::Manager::getComponent<TransformComponent>(_id).pos;
}

bool Unit::selected() const {
  return ECS::Manager::getComponent<SelectableComponent>(_id).selected;
}

Path& Unit::path() const {
  return ECS::Manager::getComponent<MotionComponent>(_id).path;
}

glm::ivec2 Unit::currentTarget() const {
  return ECS::Manager::getComponent<MotionComponent>(_id).currentTarget.operator*();
}
