#pragma once

#include "Enemy.h"
#include "Graphics.h"
#include "Region.h"
#include "RegionGenerator.h"
#include "Structure.h"
#include "Unit.h"

#include <optional>

class World {
  Region _region; // this should be a square

  std::vector<Unit> _units;
  std::vector<Enemy> _enemies;
  std::vector<Structure> _structures;

  ResourceType& _resources;

  void _drawUnits(TextureBatch& batch) const;
  void _drawEnemies(TextureBatch& batch) const;
  void _drawStructures(TextureBatch& batch) const;
  void _drawDebug(View& view) const;

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
  World(size_t size, ResourceType& resources) : _region({size, std::vector<Tile>(size, Tile::GRASS)}), _resources(resources) {
    RegionGenerator().generate(_region);
  }

  World& operator=(World&& other) {
    _units = other._units;
    _enemies = other._enemies;
    _structures = other._structures;
    _resources = other._resources;
    
    return *this;
  }

  Region& region() {
    return _region;
  }

  static void tileHolo(View& view, glm::ivec2 tile_index) {
    glm::vec2 offset(-0.5, -0.5);

    RectangleBatch()
        .add()
        .position(glm::vec2(tile_index.x * tile_size, tile_index.y * tile_size) -
                  offset * tile_size)
        .color({.7, .7, .7, .5})
        .size({tile_size, tile_size})
        .draw(view);
  }

  void structHolo(View& view, glm::ivec2 p) {
    const glm::vec2 offset(-0.5, -0.5);
    const glm::vec4 buildColor{.7, .7, .7, .5};
    const glm::vec4 occupiedColor{1, .3, .3, .5};

    const glm::vec4 color = _region.structureAt(p) ? occupiedColor : buildColor;

    RectangleBatch()
        .add()
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

  virtual void draw(TextureBatch& batch, View& view, bool debug) {
    batch.clear();
    batch.view(view);

    _region.draw(batch);
    _drawStructures(batch);
    _drawEnemies(batch);
    _drawUnits(batch);

    batch.update();
    batch.draw();
    
    if (debug) {
      _drawDebug(view);
    }
  }

  bool addUnit(glm::vec2 pos);
  bool addEnemy(glm::vec2 pos);
  bool addStructure(glm::ivec2 cell);

  bool removeUnit(ECS::Entity id);
  bool removeEnemy(ECS::Entity id);
  bool sellStructure(glm::ivec2 cell);

  std::optional<Structure> structureAt(glm::ivec2 cell);
  // bool unitAt(glm::ivec2 cell);
};
