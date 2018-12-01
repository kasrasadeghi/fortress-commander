#include "World.h"
#include "Graphics.h"
#include "Unit.h"
#include "Game.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// clang-format off
void mat4_print(glm::mat4 m) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::cout << m[j][i] << "  ";
    }
    std::cout << std::endl;
  }
}
// clang-format on

void World::_drawUnits(View& view) const {
  const glm::vec4 selectedCol{1, 1, 0, 1}, unselectedCol{1, 0, 0, 1};
  CircleBatch circles;

  constexpr float pathMarkerSize = 0.8;
  RectangleBatch rectangles;
  glm::vec2 pathTileSize(tile_size * pathMarkerSize, tile_size * pathMarkerSize);

  glm::vec2 pathTileOffset(0.5 * pathMarkerSize * tile_size, 0.5 * pathMarkerSize * tile_size);
  pathTileOffset -= glm::vec2(0.5, 0.5) * (tile_size * pathMarkerSize);

  // clang-format off
  for (auto& u : _units) {
    circles.add()
      .position(u.pos())
      .size({tile_size, tile_size})
      .color(u.selected() ? selectedCol : unselectedCol);
    
    auto& path = u.path();
    for (auto p : path) {
      rectangles.add()
        .position(Game::centerOfTile(p) - pathTileOffset)
        .size(pathTileSize)
        .color({1, 0, 0, 0.3});
    }

    if (not path.empty()) {
      auto target = u.currentTarget();
      rectangles.add()
        .position(Game::centerOfTile(target) - pathTileOffset)
        .size(pathTileSize)
        .color({0, 0, 1, 0.3});
    }
  }
  // clang-format on

  circles.draw(view);
  rectangles.draw(view);
}

void World::_drawEnemies(View& view) const {
  const glm::vec4 enemyCol{1, 0, 1, 1};
  CircleBatch circles;

  constexpr float pathMarkerSize = 0.8;
  RectangleBatch rectangles;
  glm::vec2 pathTileSize(tile_size * pathMarkerSize, tile_size * pathMarkerSize);

  glm::vec2 pathTileOffset(0.5 * pathMarkerSize * tile_size, 0.5 * pathMarkerSize * tile_size);
  pathTileOffset -= glm::vec2(0.5, 0.5) * (tile_size * pathMarkerSize);

  // clang-format off
  for (auto& e : _enemies) {
    circles.add()
      .position(e.pos())
      .size({tile_size, tile_size})
      .color(enemyCol);
    
    auto& path = e.path();
    for (auto p : path) {
      rectangles.add()
        .position(Game::centerOfTile(p) - pathTileOffset)
        .size(pathTileSize)
        .color({1, 0, 0, 0.3});
    }

    if (not path.empty()) {
      auto target = e.currentTarget();
      rectangles.add()
        .position(Game::centerOfTile(target) - pathTileOffset)
        .size(pathTileSize)
        .color({0, 0, 1, 0.3});
      
      LineBatch()
        .add()
          .points(e.pos(), Game::centerOfTile(target))
          .lineWidth(0.2)
          .color({1, 0, 1, 1})
        .draw(view);
    }
  }
  // clang-format on

  circles.draw(view);
  rectangles.draw(view);
}

void World::_drawStructures(View& view) const {
  const glm::vec4 color {0.5, 0.5, 0.5, 1};

  const glm::vec2 offset(-tile_size * 0.5, -tile_size * 0.5);
  RectangleBatch rb;

  for (const auto& structure : _structures) {
    rb.add()
      .color(color)
      .size({tile_size, tile_size})
      .position(structure.pos() * tile_size - offset);
  }

  rb.draw(view);
}

void World::addUnit(glm::vec2 pos) {
  _units.emplace_back(pos, *this);
}

void World::addEnemy(glm::vec2 pos) {
  _enemies.emplace_back(pos, *this);
  _enemies.back().pathTo({world_size / 2.f, world_size / 2.f});
}

void World::addStructure(glm::ivec2 cell) {
  _structures.emplace_back(cell, *this);
  _region.addStructure(cell);

  for (auto& u : _units) {
    u.repath();
  }

  for (auto& u : _enemies) {
    u.repath();
  }
}

void World::removeUnit(ECS::Entity id) {
  for (auto iter = _units.begin(); iter < _units.end(); ++iter) {
    if (iter->id == id) {
      _units.erase(iter);
      break;
    }
  }

  ECS::Manager::deleteEntity(id);
}

void World::removeEnemy(ECS::Entity id) {
  for (auto iter = _enemies.begin(); iter < _enemies.end(); ++iter) {
    if (iter->id == id) {
      _enemies.erase(iter);
      break;
    }
  }

  ECS::Manager::deleteEntity(id);
}

void World::sellStructure(glm::ivec2 cell) {
  for (auto iter = _structures.begin(); iter < _structures.end(); ++iter) {
    if (iter->pos() == glm::vec2{cell}) {
      ECS::Manager::deleteEntity(iter->id);
      iter = _structures.erase(iter);

      break;
    }
  }

  _region.removeStructure(cell);

  for (auto& u : _units) {
    u.repath();
  }

  for (auto& u : _enemies) {
    u.repath();
  }
}
