#pragma once

#include "Config.h"
#include "Region.h"

#include "GlmHashes.h"

#include <glm/gtc/matrix_transform.hpp>

#include <random>

class RegionGenerator {
  constexpr static float world_bounds = world_size * tile_size;

  std::mt19937 _mt;
  std::uniform_int_distribution<int> _terrainGen{0, 2};

  RegionGenerator() : _mt((std::random_device{})()) {}

  static RegionGenerator& _getInstance() {
    static RegionGenerator instance;

    return instance;
  }

  // TODO: Use Perlin noise to generate a good region
  void _generate(Region& region) {
    auto& data = region._data;

    if (data.empty() || data[0].empty()) {
      throw std::runtime_error("Bad region");
    }

    for (size_t x = 0; x < data.size(); ++x) {
      for (size_t y = 0; y < data[0].size(); ++y) {
        auto tile = _terrainGen(_mt);

        if (tile == 1) {
          //data[x][y] = Tile::WATER;
        }
      }
    }
  }

public:
  static void generate(Region& region) {
    _getInstance()._generate(region);
  }
};
