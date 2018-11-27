#include "Unit.h"
#include "Game.h"

Unit::Unit(glm::vec2 pos, ECS::Manager& manager)
    : _target(pos), _manager(manager), _id(_manager.createEntity()) {

  ECS::Entity id = _id; // have to do this because this pointer gets moved around

  _manager.addComponent<TransformComponent>(_id, TransformComponent(pos, 0.f));
  _manager.addComponent<MotionComponent>(_id, MotionComponent());
  _manager.addComponent<SelectableComponent>(_id, SelectableComponent());
  _manager.addComponent<CommandableComponent>(
      _id, CommandableComponent([&manager, id](glm::vec2 pos) {
        manager.getComponent<MotionComponent>(id).pathTo(pos);
      }));
  _manager.registerEntity(_id);
}