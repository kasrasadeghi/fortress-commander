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
  circles.size({tile_size, tile_size});

  constexpr float pathMarkerSize = 0.8;
  RectangleBatch rectangles;
  rectangles.size({tile_size * pathMarkerSize, tile_size * pathMarkerSize});

  glm::vec2 pathTileOffset(0.5 * pathMarkerSize * tile_size, 0.5 * pathMarkerSize * tile_size);
  pathTileOffset -= glm::vec2(0.5, 0.5) * (tile_size * pathMarkerSize);

  // clang-format off
  for (auto& u : _units) {
    circles.add()
      .position(u.pos())
      .color(u.selected() ? selectedCol : unselectedCol);
    
    auto& path = u.path();
    for (auto p : path) {
      rectangles.add()
        .position(Game::centerOfTile(p) - pathTileOffset)
        .color({1, 0, 0, 0.3});
    }

    if (not path.empty()) {
      auto target = u.currentTarget();
      rectangles.add()
        .position(Game::centerOfTile(target) - pathTileOffset)
        .color({0, 0, 1, 0.3});
    }
  }
  // clang-format on

  circles.draw(view);
  rectangles.draw(view);
}

void World::_drawEnemies(View& view) const {
  const glm::vec4 enemyColor{1, 0, 1, 1};
  CircleBatch circles;
  circles.size({tile_size, tile_size});

  RectangleBatch rectangles;
  rectangles.size({tile_size * 0.8, tile_size * 0.8});

  glm::vec2 pathTileOffset(0.4 * tile_size, 0.4 * tile_size);

  // clang-format off
  for (auto& e : _enemies) {
    circles.add()
      .position(e.pos())
      .color(enemyColor);
    
    auto& path = e.path();
    for (auto p : path) {
      rectangles.add()
        .position(Game::centerOfTile(p) - pathTileOffset)
        .color({1, 0, 0, 0.3});
    }

    if (not path.empty()) {
      auto target = e.currentTarget();
      rectangles.add()
        .position(Game::centerOfTile(target) - pathTileOffset)
        .color({0, 0, 1, 0.3});
    }
  }
  // clang-format on

  circles.draw(view);
  rectangles.draw(view);
}

void World::addUnit(glm::vec2 pos) {
  _units.emplace_back(pos, *this);
}

void World::addEnemy(glm::vec2 pos) {
  _enemies.emplace_back(pos, *this);
  _enemies.back().pathTo({world_size / 2.f, world_size / 2.f});
}
