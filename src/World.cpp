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
  const glm::vec4 selectedCol{1, 1, 0, 1}, unselectedCol{1, 0, 0, 1}, attackingColor{1, 1, 1, 1};

  // clang-format off
  for (auto& u : _units) {
    float healthPercent = (float)u.health()/(float)u.max_health;
    auto& attack = ECS::Manager::getComponent<AttackComponent>(u.id);
    
    //TODO: make bullets flash instead of selection

    float angle = ECS::Manager::getComponent<TransformComponent>(u.id).rot;

    auto baseColor = unselectedCol;
    if (u.selected()) {
      baseColor = selectedCol;
    } else if (attack.timer > attack.cooldown - muzzleFlashTime) {
      baseColor = attackingColor;
    }

    baseColor.a *= healthPercent;

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
  const glm::vec4 enemyCol{1, 0, 1, 1}, attackingColor{1, 1, 1, 1};

  // clang-format off
  for (auto& e : _enemies) {
    float healthPercent = (float)e.health()/(float)e.max_health;
    auto& attack = ECS::Manager::getComponent<AttackComponent>(e.id);

    float angle = ECS::Manager::getComponent<TransformComponent>(e.id).rot;

    auto baseColor = enemyCol;
    if (attack.timer > attack.cooldown - muzzleFlashTime) {
      baseColor = attackingColor;
    }
    baseColor.a *= healthPercent;

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

  for (auto& u : _units) {
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
  rectangles.draw(view);
}

void World::_drawStructures(TextureBatch& batch) const {
  const glm::vec2 offset(-tile_size * 0.5, -tile_size * 0.5);

  for (const auto& structure : _structures) {
    batch.add(TextureBatch::Instance{
      .pos = structure.pos() * tile_size - offset,
      .size = {tile_size, tile_size},
      .aColor = {1, structure.percentHealth(), structure.percentHealth(), 1},
      .texOffset = 2.f,
    });
  }
}

bool World::addUnit(glm::vec2 pos) {
  if (not _region.inBounds(pos)) {
    return false;
  }

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

bool World::addStructure(glm::ivec2 cell) {
  if (not _region.inBounds({cell.x, cell.y})) {
    return false;
  }

  _structures.emplace_back(cell, *this);
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

bool World::sellStructure(glm::ivec2 cell) {
  // TODO: get money somehow # resources

  bool found = false;

  ECS::Entity entity = ECS::InvalidEntityId;
  for (auto iter = _structures.begin(); iter < _structures.end(); ++iter) {
    if (iter->pos() == glm::vec2{cell}) {
      entity = iter->id;
      iter = _structures.erase(iter);
      found = true;

      break;
    }
  }

  return found && removeStructure(entity, cell);
}

bool World::removeStructure(ECS::Entity id) {
  glm::ivec2 cell {-1, -1};
  bool found = false;

  for (auto iter = _structures.begin(); iter < _structures.end(); ++iter) {
    if (iter->id == id) {
      cell = Game::mapCoordsToTile(iter->pos());
      iter = _structures.erase(iter);
      found = true;

      break;
    }
  }

  return found && removeStructure(id, cell);
}

bool World::removeStructure(ECS::Entity id, glm::ivec2 cell) {
  
  bool result = ECS::Manager::deleteEntity(id);

  _region.removeStructure(cell);

  for (auto& u : _units) {
    u.repath();
  }

  for (auto& e : _enemies) {
    e.repath();
  }

  return result;
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
