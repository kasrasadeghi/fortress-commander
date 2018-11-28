#include "Unit.h"
#include "Game.h"

Unit::Unit(glm::vec2 pos) : _target(pos), _id(ECS::Manager::createEntity()) {

  ECS::Entity id = _id; // have to do this because this pointer gets moved around

  ECS::Manager::addComponent<TransformComponent>(_id, TransformComponent(pos, 0.f));
  ECS::Manager::addComponent<MotionComponent>(_id, MotionComponent());
  ECS::Manager::addComponent<SelectableComponent>(_id, SelectableComponent());
  ECS::Manager::addComponent<CommandableComponent>(
      _id, CommandableComponent([id](glm::vec2 pos) {
        ECS::Manager::getComponent<MotionComponent>(id).pathTo(pos);
      }));
  ECS::Manager::registerEntity(_id);
}
