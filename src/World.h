#pragma once

#include "Graphics.h"
#include "Unit.h"

enum class Tile { NONE, GRASS, WATER };

class World /* : public sf::Drawable */ {
  std::vector<std::vector<Tile>> _region; // this should be a square

  std::vector<Unit> _units;
  // std::vector<Structure> _structures;

  void _drawRegion(View& view) const;
  void _drawUnits(View& view) const;

  // stuff to make out of bounds clicks snap back to bounds
  bool _snapToRegion(glm::ivec2& v) {
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

  std::vector<std::vector<Tile>>& region() { return _region; }

  static void tileHolo(View& view, glm::ivec2 tile_index) {
    InstancedRectangle r(tile_index.x * tile_size, tile_index.y * tile_size);
    r.color(.7, .7, .7, .5);
    r.size(tile_size, tile_size);
    r.draw(view);
  }

  Tile flipCell(glm::ivec2 v) {
    _snapToRegion(v);
    return _region[v.x][v.y] = (_region[v.x][v.y] == Tile::GRASS) ? Tile::WATER : Tile::GRASS;
  }

  void setCell(glm::ivec2 v, Tile t) {
    _snapToRegion(v);
    _region[v.x][v.y] = t;
  }

  virtual void draw(View& view) const {
    _drawRegion(view);
    _drawUnits(view);
  }

  void addUnit(glm::vec2 pos);
};