#pragma once

#include "Graphics.h"
#include "Unit.h"
#include "Enemy.h"
#include "Structure.h"
#include "Region.h"


class World {
  Region _region; // this should be a square

  std::vector<Unit> _units;
  std::vector<Enemy> _enemies;
  std::vector<Structure> _structures;

  void _drawUnits(View& view) const;
  void _drawEnemies(View& view) const;
  void _drawStructures(View& view) const;

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
  World(size_t size) : _region({size, std::vector<Tile>(size, Tile::GRASS)}) {}

  Region& region() { return _region; }

  static void tileHolo(View& view, glm::ivec2 tile_index) {
    glm::vec2 offset(-0.5, -0.5);

    RectangleBatch().add()
      .position(glm::vec2(tile_index.x * tile_size, tile_index.y * tile_size) - offset * tile_size)
      .color({.7, .7, .7, .5})
      .size({tile_size, tile_size})
      .draw(view);
  }

  void structHolo(View& view, glm::ivec2 p) {
    const glm::vec2 offset(-0.5, -0.5);
    const glm::vec4 buildColor {.7, .7, .7, .5};
    const glm::vec4 occupiedColor {1, .3, .3, .5};

    const glm::vec4 color = _region.structureAt(p) ? occupiedColor : buildColor;

    RectangleBatch().add()
      .position(glm::vec2(p.x * tile_size, p.y * tile_size) - offset * tile_size)
      .color(color)
      .size({tile_size, tile_size})
      .draw(view);
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
    _region.draw(view);
    _drawStructures(view);
    _drawEnemies(view);
    _drawUnits(view);
  }

  void addUnit(glm::vec2 pos);
  void addEnemy(glm::vec2 pos);
  void addStructure(glm::ivec2 cell);

  // bool unitAt(glm::ivec2 cell);
};
