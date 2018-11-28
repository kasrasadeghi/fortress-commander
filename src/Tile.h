
#include <glm/glm.hpp>
#include <unordered_map>

enum class Tile { NONE, GRASS, WATER };

struct TileType {
  glm::vec4 color;
  bool walkable;
};

class TileProperties {
  static const std::unordered_map<Tile, const TileType> _data;

public:
  static const TileType& of(Tile t) {
    return _data.at(t);
  }
};
