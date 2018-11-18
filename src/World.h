#pragma once

#include <SFML/Graphics.hpp>

#include "Unit.h"

enum class Tile { NONE, GRASS, WATER };

class World : public sf::Drawable {
  std::vector<std::vector<Tile>> _region; // this should be a square

  std::vector<Unit> _units;
  // std::vector<Structure> _structures;

  void _drawRegion(sf::RenderTarget& window) const;
  void _drawUnits(sf::RenderTarget& window) const;

  /// stuff to make out of bounds clicks snap back to bounds
  bool _snapToRegion(sf::Vector2i& v) {
    bool result = true;
    if (0 > v.x) {
      v.x = 0;
      result = false;
    }
    if (v.x >= world_size) {
      v.x = world_size - 1;
      result = false;
    }
    if (0 > v.y) {
      v.y = 0;
      result = false;
    }
    if (v.y >= world_size) {
      v.y = world_size - 1;
      result = false;
    }
    return result;
  }

  friend class Game;

public:
  constexpr static int world_size = 100;

  World(size_t size) : _region(size, std::vector<Tile>(size, Tile::GRASS)) {}

  Tile flipCell(sf::Vector2i v) {
    _snapToRegion(v);
    return _region[v.x][v.y] =
               (_region[v.x][v.y] == Tile::GRASS) ? Tile::WATER : Tile::GRASS;
  }

  void setCell(sf::Vector2i v, Tile t) {
    _snapToRegion(v);
    _region[v.x][v.y] = t;
  }

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const {
    _drawRegion(rw);
    _drawUnits(rw);
  }
};