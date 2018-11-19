#pragma once

#include "Config.h"

#include <SFML/Graphics.hpp>

class Unit : public sf::Drawable {
  sf::Vector2f _pos;
  sf::Vector2f _target;

public:
  constexpr static float unit_size = 0.5f * tile_size;
  constexpr static float unit_speed = 2.f;

  Unit(sf::Vector2f pos) : _pos(pos), _target(pos) {}

  static sf::CircleShape holo(sf::Vector2f curr) {
    sf::CircleShape r(Unit::unit_size);
    r.setPosition(curr.x - Unit::unit_size, curr.y - Unit::unit_size);
    r.setFillColor(sf::Color(255, 200, 200, 150));
    return r;
  }

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const;

  void pathTo(sf::Vector2f coords);
  void update(const sf::Time& dt);
};
