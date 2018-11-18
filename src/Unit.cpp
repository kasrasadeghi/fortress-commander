#include "Unit.h"
#include "VecMath.h"
#include "World.h"

void Unit::draw(sf::RenderTarget& rw, sf::RenderStates states) const {
  sf::CircleShape c(unit_size);
  c.setFillColor(sf::Color::Red);
  c.setPosition(_pos - sf::Vector2f(unit_size, unit_size));

  rw.draw(c);
}

void Unit::pathTo(sf::Vector2f coords) { _target = coords; }

void Unit::update(const sf::Time& dt) {
  if (_target == _pos) return;

  const auto dir = norm(_target - _pos);

  if (magn(_target - _pos) > dt.asSeconds() * unit_speed) {
    _pos += dir * dt.asSeconds() * unit_speed;
  } else {
    _pos = _target;
  }
}
