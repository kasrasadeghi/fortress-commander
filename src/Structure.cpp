#include "Structure.h"
#include "Game.h"

#include "World.h"

Structure::Structure(glm::vec2 pos, World& world) : id(ECS::Manager::createEntity()) {
  ECS::Manager::addComponent<TransformComponent>(id, TransformComponent(world, pos, 0.f));
  ECS::Manager::addComponent<HealthComponent>(id, HealthComponent(max_health));

  ECS::Manager::registerEntity(id);
}

glm::vec2 Structure::pos() const {
  return ECS::Manager::getComponent<TransformComponent>(id).pos;
}

HealthValue Structure::health() const {
  return ECS::Manager::getComponent<HealthComponent>(id).health;
}

float Structure::percentHealth() const {
  return (float)(health()) / (float)(Structure::max_health);
}