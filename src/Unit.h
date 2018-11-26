#pragma once

#include "ECS/Manager.h"

#include <SFML/Graphics.hpp>

// enum class UnitType { FRIENDLY, HOSTILE };

class Unit : public sf::Drawable {
  // UnitType _type; // friendly or hostile
  sf::Vector2f _target;
  ECS::Manager* _manager;

public:
  const ECS::Entity _id;

  constexpr static float unit_size = 0.5f;

  Unit(sf::Vector2f pos, ECS::Manager* manager /*, UnitType type*/);

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const;
};
