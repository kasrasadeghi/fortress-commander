#pragma once

#include "Graphics.h"
#include "Unit.h"

enum class Tile { NONE, GRASS, WATER };

class World {
  std::vector<std::vector<Tile>> _region; // this should be a square

  std::vector<Unit> _units;
  // std::vector<Structure> _structures;

  void _drawRegion(View& view) const;
  void _drawUnits(View& view, const std::vector<uint>& selected_units) const;

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
  using Region = std::vector<std::vector<Tile>>;

  World(size_t size) : _region(size, std::vector<Tile>(size, Tile::NONE)) {
    for (int i = 0; i < world_size; ++i) {
      for (int j = 0; j < world_size; ++j) {
        if ((i + j) % 2) {
          _region[i][j] = Tile::GRASS;
        } else {
          _region[i][j] = Tile::WATER;
        }
      }
    }
  }
  
  Region& region() { return _region; }

  static void tileHolo(View& view, glm::ivec2 tile_index) {
    InstancedRectangle r(tile_index.x * tile_size, tile_index.y * tile_size);
    r.color(.7, .7, .7 /*TODO:, .5 */);
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

  virtual void draw(View view, const std::vector<uint>& selected_units) const {
    _drawRegion(view);
    _drawUnits(view, selected_units);
  }

  std::optional<uint> unitAt(glm::vec2 coords) {
    for (auto&& u: _units) {
      if (glm::distance(u.pos(), coords) < Unit::unit_size) {
        return std::optional(u.ID);
      }
    }
    return std::optional<uint>();
  }

  std::vector<uint> unitsIn(glm::vec2 start, glm::vec2 end) {
    if (start.x < end.x) std::swap(start, end);
    glm::vec2 midpoint = (start + end) / glm::vec2(2, 2);
    glm::vec2 axes = (start - midpoint);

    auto left = midpoint.x - axes.x;
    auto right = midpoint.x + axes.x;
    auto top = midpoint.y - axes.y;
    auto bottom = midpoint.y + axes.y;

    // *s         ^axes.y
    // <-axes.x   *m
    //                       *e

    std::vector<uint> ids;
    for (auto&& u : _units) {
      auto pos = u.pos();
      auto us = Unit::unit_size;
      auto contains = pos.x + us >= left && pos.x - us <= right && pos.y - us <= top && pos.y + us >= bottom;
      if (contains) {
        ids.push_back(u.ID);
      }
    }
    return ids;
  }
};