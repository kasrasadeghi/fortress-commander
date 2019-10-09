#pragma once

#include "Config.h"
#include "World.h"

#include "GlmHashes.h"

#include <glm/gtc/matrix_transform.hpp>

#include <random>

class EnemySpawner {
  constexpr static float world_bounds = world_size * tile_size;
  constexpr static float spawn_interval = 5;      // seconds
  constexpr static int spawn_group_count = 1;     // number of groups
  constexpr static int spawn_group_size = 3;      // max enemies per group
  constexpr static int spawn_tiles_from_edge = 4; // offset of group from edge of map
  constexpr static float spawn_group_radius = 3;

  std::mt19937 _mt;
  World& _world;
  float _timer = spawn_interval;

  std::uniform_int_distribution<int> _sideDist{0, 3};
  std::uniform_real_distribution<float> _fdist{tile_size / 2.f, world_bounds - tile_size / 2.f};
  std::uniform_real_distribution<float> _angleDist{0, glm::pi<float>() * 2};

  float _pos() {
    return _fdist(_mt);
  }
  int _side() {
    return _sideDist(_mt);
  }
  glm::vec2 _spawnOffset() {
    float dir = _angleDist(_mt);
    float len = std::uniform_real_distribution<float>(0, spawn_group_radius)(_mt);
    return glm::vec2(cos(dir) * len, sin(dir) * len);
  }

public: // seed with random device
  EnemySpawner(World& world) : _mt((std::random_device{})()), _world(world) {}

  void update(float dt) {
    _timer -= dt;
    if (_timer <= 0) {
      spawn();
      _timer = spawn_interval;
    }
  }

  void spawn() {
    for (int i = 0; i < spawn_group_count; ++i) {
      int side = _side();

      glm::vec2 pos{0, 0};
      if (side == 0) {
        pos = {tile_size / 2.f + spawn_tiles_from_edge, _pos()};
      }
      if (side == 1) {
        pos = {_pos(), tile_size / 2.f + spawn_tiles_from_edge};
      }
      if (side == 2) {
        pos = {world_bounds - (tile_size / 2.f + spawn_tiles_from_edge), _pos()};
      }
      if (side == 3) {
        pos = {_pos(), world_bounds - (tile_size / 2.f + spawn_tiles_from_edge)};
      }

      for (int j = 0; j < spawn_group_size; ++j) {
        auto offsetPos = pos + _spawnOffset();
        if (offsetPos.x < 0 || offsetPos.y < 0 || offsetPos.x >= world_bounds ||
            offsetPos.y >= world_bounds) {
          continue;
        }
        _world.addEnemy(offsetPos);
      }
    }
  }

  void reset() {
    _timer = spawn_interval;
  }
};
