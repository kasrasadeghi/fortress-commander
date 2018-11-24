#pragma once

#include "Config.h"
#include "Graphics.h"

// #include <SFML/Graphics.hpp>

class Unit {
  glm::vec2 _pos;
  glm::vec2 _target;

public:
  constexpr static float unit_size = 0.5f * tile_size;
  constexpr static float unit_speed = 2.f;

  Unit(glm::vec2 pos) : _pos(pos), _target(pos) {}

  glm::vec2 pos() const { return _pos; }

  // static sf::CircleShape holo(glm::vec curr) {
  //   sf::CircleShape r(Unit::unit_size);
  //   r.setPosition(curr.x - Unit::unit_size, curr.y - Unit::unit_size);
  //   r.setFillColor(sf::Color(255, 200, 200, 150));
  //   return r;
  // }

  void pathTo(glm::vec2 coords);
  void update(float dt);
};
