#include "Unit.h"

#include "Game.h"
#include "VecMath.h"

Unit::Unit(sf::Vector2f pos, ECS::Manager* manager /*, UnitType type*/)
    : _target(pos), _manager(manager),
      _id(_manager->createEntity()) /*, _type(type)*/ {

  _manager->addComponent<TransformComponent>(_id, TransformComponent(pos, 0.f));
  _manager->addComponent<MotionComponent>(_id, MotionComponent());
  _manager->addComponent<SelectableComponent>(_id, SelectableComponent());
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

void Unit::pathTo(sf::Vector2f coords) { _target = coords; }

void Unit::update(const sf::Time& dt) {
  // TODO: move unit updates to a system
  sf::Vector2f& pos = _manager->getComponent<TransformComponent>(_id).pos;
  sf::Vector2f& vel = _manager->getComponent<MotionComponent>(_id).velocity;
  const float speed = magn(vel);

  sf::Vector2f target_dx(_target.x - pos.x, _target.y - pos.y);
  float len = magn(target_dx);
  if (len < speed * dt.asSeconds()) {
    pos = _target;
    vel = sf::Vector2f(0, 0);
  } else if (len > 0) {
    float vx = target_dx.x / len * unit_speed;
    float vy = target_dx.y / len * unit_speed;
    vel = sf::Vector2f(vx, vy);
  }
}
