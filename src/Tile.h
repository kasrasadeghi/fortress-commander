#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

enum class Tile { NONE, GRASS, SAND, WATER };

struct TileType {
  glm::vec4 color;
  bool walkable;
  int texOffset;
};

class TileProperties {
  static const std::unordered_map<Tile, const TileType> _data;

public:
  static const TileType& of(Tile t) {
    return TileProperties::_data.at(t);
  }
};
