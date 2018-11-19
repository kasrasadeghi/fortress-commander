#pragma once

#include "ECS/Manager.h"

#include <SFML/Graphics.hpp>

// enum class UnitType { FRIENDLY, HOSTILE };

class Unit : public sf::Drawable {
  sf::Vector2f _pos;
  // UnitType _type; // friendly or hostile
  sf::Vector2f _target;
  ECS::Manager* _manager;

  ECS::Entity _id;

public:
  constexpr static float unit_size = 0.5f;
  constexpr static float unit_speed = 2.f;

  Unit(sf::Vector2f pos, ECS::Manager* manager /*, UnitType type*/);

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const;

  void pathTo(sf::Vector2f coords);
  void update(const sf::Time& dt);
};
