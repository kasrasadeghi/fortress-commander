#include "Unit.h"

#include "Game.h"
#include "VecMath.h"

Unit::Unit(sf::Vector2f pos, ECS::Manager* manager /*, UnitType type*/)
    : _target(pos), _manager(manager),
      _id(_manager->createEntity()) /*, _type(type)*/ {

  ECS::Entity id = _id; // have to do this because this pointer gets moved around

  _manager->addComponent<TransformComponent>(_id, TransformComponent(pos, 0.f));
  _manager->addComponent<MotionComponent>(_id, MotionComponent());
  _manager->addComponent<SelectableComponent>(_id, SelectableComponent());
  _manager->addComponent<CommandableComponent>(_id, CommandableComponent([manager, id](sf::Vector2f pos){
    manager->getComponent<MotionComponent>(id).pathTo(pos);
  }));
  _manager->registerEntity(_id);
}

void Unit::draw(sf::RenderTarget& rw, sf::RenderStates states) const {
  constexpr auto radius = unit_size * World::tile_size;
  const sf::Vector2f pos = _manager->getComponent<TransformComponent>(_id).pos;
  sf::CircleShape c(unit_size * World::tile_size);
  c.setFillColor(sf::Color::Red);
  if (_manager->getComponent<SelectableComponent>(_id).selected) {
    c.setFillColor(sf::Color::Magenta);
  }
  c.setPosition(pos - sf::Vector2f(radius, radius));

  rw.draw(c);
}
