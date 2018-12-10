#include "World.h"
#include "Game.h"
#include "Graphics.h"
#include "Unit.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <glm/trigonometric.hpp>

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

void World::_drawUnits(TextureBatch& batch) const {
  const glm::vec4 selectedCol{.53, .53, .82, 1}, unselectedCol{.2, .4, .6, 1}, attackingColor{1, 1, 1, 1};

  // clang-format off
  for (auto& u : _units) {
    float attackTimer = ECS::Manager::getComponent<AttackComponent>(u.id).attackTimer;
    
    //TODO: make bullets flash instead of selection

    float angle = ECS::Manager::getComponent<TransformComponent>(u.id).rot;

    auto baseColor = unselectedCol;
    if (u.selected()) {
      baseColor = selectedCol;
    } else if (attackTimer < muzzleFlashTime) {
      baseColor = attackingColor;
    }

    batch.add(TextureBatch::Instance{
      .pos = u.pos(),
      .size = {tile_size, tile_size},
      .aColor = baseColor,
      .rotation = angle,
    });
    
  }
  // clang-format on
}

void World::_drawEnemies(TextureBatch& batch) const {
  const glm::vec4 enemyCol{.85, .36, .22, 1}, attackingColor{1, 1, 1, 1};

  // clang-format off
  for (auto& e : _enemies) {
    float attackTimer = ECS::Manager::getComponent<AttackComponent>(e.id).attackTimer;

    float angle = ECS::Manager::getComponent<TransformComponent>(e.id).rot;

    auto baseColor = enemyCol;
    if (attackTimer < muzzleFlashTime) {
      baseColor = attackingColor;
    }

    batch.add(TextureBatch::Instance{
      .pos = e.pos(),
      .size = {tile_size, tile_size},
      .aColor = baseColor,
      .rotation = angle,
    });
  }
  // clang-format on
}

void World::_drawDebug(View& view) const {
  RectangleBatch rectangles;

  constexpr float pathMarkerSize = 0.8;
  glm::vec2 pathTileSize(tile_size * pathMarkerSize, tile_size * pathMarkerSize);

  glm::vec2 pathTileOffset(0.5 * pathMarkerSize * tile_size, 0.5 * pathMarkerSize * tile_size);
  pathTileOffset -= glm::vec2(0.5, 0.5) * (tile_size * pathMarkerSize);

  for (auto& e : _enemies) {
    auto& path = e.path();
    for (auto p : path) {
      rectangles.add()
        .position(Game::centerOfTile(p) - pathTileOffset)
        .size(pathTileSize)
        .color({1, 0, 0, 0.3});
    }

    if (not path.empty()) {
      auto target = path.front();
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

  for (auto& u : _units) {
    auto& path = u.path();
    for (auto p : path) {
      rectangles.add()
        .position(Game::centerOfTile(p) - pathTileOffset)
        .size(pathTileSize)
        .color({1, 0, 0, 0.3});
    }

    if (not path.empty()) {
      auto target = path.front();
      rectangles.add()
        .position(Game::centerOfTile(target) - pathTileOffset)
        .size(pathTileSize)
        .color({0, 0, 1, 0.3});
    }
  }
  rectangles.draw(view);
}

void World::_drawStructures(TextureBatch& batch) const {
  const glm::vec2 offset(-tile_size * 0.5, -tile_size * 0.5);

  for (const auto& structure : _structures) {
    batch.add(TextureBatch::Instance{
      .pos = structure.pos() * tile_size - offset,
      .size = {tile_size, tile_size},
      .texOffset = static_cast<float>(structure.texOffset)
    });
  }
}

bool World::addUnit(glm::vec2 pos) {
  if (not _region.inBounds(pos)) {
    return false;
  }

  if (_resources < Unit::cost) {
    return false;
  }

  // Subtract the cost from the player's resource account
  _resources -= Unit::cost;

  _units.emplace_back(pos, *this);
  return true;
}

bool World::addEnemy(glm::vec2 pos) {
  if (not _region.inBounds(pos)) {
    return false;
  }

  _enemies.emplace_back(pos, *this);
  _enemies.back().pathTo({world_size / 2.f, world_size / 2.f});
  return true;
}

bool World::addStructure(glm::ivec2 cell, StructureType t) {
  if (not _region.inBounds({cell.x, cell.y}) or cell == glm::ivec2{world_size / 2, world_size / 2}) {
    return false;
  }

  auto cost = StructureProperties::of(t).cost;
  if (_resources < cost) {
    return false;
  }

  // Subtract the cost from the player's resource account
  _resources -= cost;

  _structures.emplace_back(cell, *this, t);
  _region.addStructure(cell);

  for (auto& u : _units) {
    u.repath();
  }

  for (auto& u : _enemies) {
    u.repath();
  }

  return true;
}

bool World::removeUnit(ECS::Entity id) {
  bool found = false;

  for (auto iter = _units.begin(); iter < _units.end(); ++iter) {
    if (iter->id == id) {
      _units.erase(iter);
      found = true;

      break;
    }
  }

  return found && ECS::Manager::deleteEntity(id);
}

bool World::removeEnemy(ECS::Entity id) {
  bool found = false;

  for (auto iter = _enemies.begin(); iter < _enemies.end(); ++iter) {
    if (iter->id == id) {
      _enemies.erase(iter);
      found = true;

      break;
    }
  }

  return found && ECS::Manager::deleteEntity(id);
}

bool World::removeStructure(ECS::Entity id) {
  bool found = false;

  for (auto iter = _structures.begin(); iter < _structures.end(); ++iter) {
    if (iter->id == id) {
      _region.removeStructure(iter->pos());
      _structures.erase(iter);
      found = true;

      break;
    }
  }

  return found && ECS::Manager::deleteEntity(id);
}

bool World::sellStructure(glm::ivec2 cell) {
  bool found = false;

  ECS::Entity id = ECS::InvalidEntityId;
  ResourceType cost;
  for (auto iter = _structures.begin(); iter < _structures.end(); ++iter) {
    if (iter->pos() == glm::vec2{cell}) {
      id = iter->id;
      cost = iter->cost;

      break;
    }
  }

  if (id == ECS::InvalidEntityId) {
    return found;
  }

  found = removeStructure(id);

  if (found) {
    _resources += cost / sell_ratio;
  }

  for (auto& u : _units) {
    u.repath();
  }

  for (auto& u : _enemies) {
    u.repath();
  }

  return found;
}

std::optional<Structure> World::structureAt(glm::ivec2 cell) {
  // TODO: Region::structureAt seemed to have stale data after Game::restart()
  // also, why is structure data split across world and region?

  for (Structure& s : _structures) {
    if (Game::mapCoordsToTile(s.pos()) == cell) {
      return s;
    }
  }
  return {};
}
