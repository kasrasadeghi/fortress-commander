#include "Enemy.h"
#include "Game.h"

#include "Path.h"
#include "World.h"

Enemy::Enemy(glm::vec2 pos, World& world) : _target(pos), id(ECS::Manager::createEntity()) {
  ECS::Manager::addComponent<TransformComponent>(id, TransformComponent(world, pos, 0.f));
  ECS::Manager::addComponent<MotionComponent>(id, MotionComponent());
  ECS::Manager::addComponent<HealthComponent>(id, HealthComponent(max_health));
  ECS::Manager::addComponent<AttackComponent>(id, AttackComponent(strength, 1.f, attackCooldown));

  ECS::Manager::registerEntity(id);
}

glm::vec2 Enemy::pos() const {
  return ECS::Manager::getComponent<TransformComponent>(id).pos;
}

Path& Enemy::path() const {
  return ECS::Manager::getComponent<MotionComponent>(id).path;
}

glm::ivec2 Enemy::currentTarget() const {
  return ECS::Manager::getComponent<MotionComponent>(id).currentTarget.operator*();
}

void Enemy::pathTo(glm::vec2 v) {
  ECS::Manager::getComponent<MotionComponent>(id).pathTo(v);
}

void Enemy::repath() const {
  return ECS::Manager::getComponent<MotionComponent>(id).repath();
}

HealthValue Enemy::health() const {
  return ECS::Manager::getComponent<HealthComponent>(id).health;
}
