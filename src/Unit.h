#pragma once

#include <SFML/Graphics.hpp>

enum class UnitType { FRIENDLY, HOSTILE };

class Unit : public sf::Drawable {
  sf::Vector2f _pos; 
  UnitType _type; // friendly or hostile

public:
  constexpr static float unit_size = 0.5f * 30; // todo tile_size, not 30

  Unit(sf::Vector2f pos/*, UnitType type*/)
    : _pos(pos)/*, _type(type)*/
  {}

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const;
};
