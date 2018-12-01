#include "Enemy.h"
#include "Game.h"

#include "World.h"
#include "Path.h"

Enemy::Enemy(glm::vec2 pos, World& world) : _target(pos), _id(ECS::Manager::createEntity()) {
  ECS::Manager::addComponent<TransformComponent>(_id, TransformComponent(world, pos, 0.f));
  ECS::Manager::addComponent<MotionComponent>(_id, MotionComponent());
  ECS::Manager::addComponent<HealthComponent>(_id, HealthComponent(health));
  ECS::Manager::addComponent<AttackComponent>(_id, AttackComponent(strength, attackCooldown));

  ECS::Manager::registerEntity(_id);
}

glm::vec2 Enemy::pos() const {
  return ECS::Manager::getComponent<TransformComponent>(_id).pos;
}

Path& Enemy::path() const {
  return ECS::Manager::getComponent<MotionComponent>(_id).path;
}

glm::ivec2 Enemy::currentTarget() const {
  return ECS::Manager::getComponent<MotionComponent>(_id).currentTarget.operator*();
}

void Enemy::pathTo(glm::vec2 v) {
  ECS::Manager::getComponent<MotionComponent>(_id).pathTo(v);
}

void Enemy::repath() const {
  return ECS::Manager::getComponent<MotionComponent>(_id).repath();  
}
