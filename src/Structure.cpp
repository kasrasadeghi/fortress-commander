#include "Structure.h"
#include "Game.h"

#include "World.h"

Structure::Structure(glm::vec2 pos, World& world) : id(ECS::Manager::createEntity()) {

  ECS::Manager::addComponent<TransformComponent>(id, TransformComponent(pos, 0.f));
  ECS::Manager::addComponent<HealthComponent>(id, HealthComponent(health));

  ECS::Manager::registerEntity(id);
}

glm::vec2 Structure::pos() const {
  return ECS::Manager::getComponent<TransformComponent>(id).pos;
}
