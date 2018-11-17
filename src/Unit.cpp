#include "Unit.h"

void Unit::draw(sf::RenderTarget& rw, sf::RenderStates states) const {
  sf::CircleShape c(unit_size);
  c.setFillColor(sf::Color::Red);
  c.setPosition(_pos);

  rw.draw(c);
}
