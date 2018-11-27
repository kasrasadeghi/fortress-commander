#include "Unit.h"
#include "Game.h"

Unit::Unit(glm::vec2 pos)
    : _target(pos), _id(ECS::Manager::getInstance().createEntity()) {

  ECS::Entity id = _id; // have to do this because this pointer gets moved around

  ECS::Manager::getInstance().addComponent<TransformComponent>(_id, TransformComponent(pos, 0.f));
  ECS::Manager::getInstance().addComponent<MotionComponent>(_id, MotionComponent());
  ECS::Manager::getInstance().addComponent<SelectableComponent>(_id, SelectableComponent());
  ECS::Manager::getInstance().addComponent<CommandableComponent>(
      _id, CommandableComponent([id](glm::vec2 pos) {
          ECS::Manager::getInstance().getComponent<MotionComponent>(id).pathTo(pos);
      }));
  ECS::Manager::getInstance().registerEntity(_id);
}
