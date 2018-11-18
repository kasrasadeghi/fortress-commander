#pragma once

#include <SFML/Graphics.hpp>

// enum class UnitType { FRIENDLY, HOSTILE };

class Unit : public sf::Drawable {
  sf::Vector2f _pos;
  // UnitType _type; // friendly or hostile
  sf::Vector2f _target;

public:
  constexpr static float unit_size = 0.5f;
  constexpr static float unit_speed = 2.f;

  Unit(sf::Vector2f pos /*, UnitType type*/)
      : _pos(pos), _target(pos) /*, _type(type)*/
  {}

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const;

  void pathTo(sf::Vector2f coords);
  void update(const sf::Time& dt);
};
