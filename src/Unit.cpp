#include "Unit.h"

#include "VecMath.h"
#include "Game.h"

Unit::Unit(sf::Vector2f pos, ECS::Manager* manager /*, UnitType type*/)
  : _pos(pos), _target(pos), _manager(manager)/*, _type(type)*/ {
  _id = _manager->createEntity();

  _manager->addComponent<TransformComponent>(_id, TransformComponent(pos, 0.f));
  _manager->addComponent<MotionComponent>(_id, MotionComponent());
  _manager->addComponent<SelectableComponent>(_id, SelectableComponent());
  _manager->registerEntity(_id);
}

void Unit::draw(sf::RenderTarget& rw, sf::RenderStates states) const {
  constexpr auto radius = unit_size * World::tile_size;
  sf::CircleShape c(unit_size * World::tile_size);
  c.setFillColor(sf::Color::Red);
  if (_manager->getComponent<SelectableComponent>(this->_id).selected) {
    c.setFillColor(sf::Color::Magenta);
  }
  c.setPosition(_pos - sf::Vector2f(radius, radius));

  rw.draw(c);
}

void Unit::pathTo(sf::Vector2f coords) { _target = coords; }

void Unit::update(const sf::Time& dt) {
  _pos = _manager->getComponentStore<TransformComponent>().get(_id).pos;
  /*
  if (_target == _pos) return;

  const auto dir = norm(_target - _pos);

  if (magn(_target - _pos) > dt.asSeconds() * unit_speed) {
    _pos += dir * dt.asSeconds() * unit_speed;
  } else {
    _pos = _target;
  }
  */
}
