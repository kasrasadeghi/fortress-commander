#include "Structure.h"
#include "Game.h"

#include "World.h"

Structure::Structure(glm::vec2 pos, World& world) : _target(pos), _id(ECS::Manager::createEntity()) {

  ECS::Manager::addComponent<TransformComponent>(_id, TransformComponent(pos, 0.f));
  ECS::Manager::addComponent<HealthComponent>(_id, HealthComponent(health));

  ECS::Manager::registerEntity(_id);
}

glm::vec2 Structure::pos() const {
  return ECS::Manager::getComponent<TransformComponent>(_id).pos;
}
