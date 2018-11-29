#pragma once

#include "Config.h"
#include "World.h"

#include <glm/gtc/matrix_transform.hpp>

#include <random>

class EnemySpawner {
  constexpr static float world_bounds = World::world_size * tile_size;
  constexpr static float spawn_interval = 5; // seconds  
  constexpr static float spawn_count = 1;

  std::mt19937 _mt;
  World& _world;
  float timer = spawn_interval;

  std::uniform_int_distribution<int> _sideDist{0, 3};
  std::uniform_real_distribution<float> _fdist{tile_size / 2.f, world_bounds - tile_size / 2.f};

  float _pos() { return _fdist(_mt); }
  int _side() { return _sideDist(_mt); }
public:                       // seed with random device
  EnemySpawner(World& world): _mt((std::random_device{})()), _world(world) {}

  void update(float dt) {
    timer -= dt;
    if (timer <= 0) {
      for (size_t i = 0; i < spawn_count; ++i) {
        spawn();
      }
      timer = spawn_interval;
    }
  }

  void spawn() {
    int side = _side();
    
    glm::vec2 pos;
    if (side == 0) {
      pos = {tile_size / 2.f, _pos()};
    }
    if (side == 1) {
      pos = {_pos(), tile_size / 2.f};
    }
    if (side == 2) {
      pos = {world_bounds - tile_size / 2.f, _pos()};
    }
    if (side == 3) {
      pos = {_pos(), world_bounds - tile_size / 2.f};
    }

    auto cell = Game::mapCoordsToTile(pos);
    if (cell.x == 100 || cell.y == 100) {
      std::printf("(%f, %f) -> (%d, %d)\n", pos.x, pos.y, cell.x, cell.y);
    }

    _world.addEnemy(pos);
  }
};