#include "Graphics.h"
#include "Tile.h"

#include <vector>

class Region {
  std::vector<std::vector<Tile>> _data;

public:
  Region(std::vector<std::vector<Tile>> data): _data(data) {}

  std::vector<Tile>& operator[](size_t i) { return _data[i]; }

  Tile& at(glm::ivec2 p) { return _data[p.x][p.y]; }

  void draw(View& view) const {
    const glm::vec4 grassCol(.3, .6, .2, 1);
    const glm::vec4 waterCol(.1, .3, .8, 1);
    RectangleBatch rects;
    rects.size({tile_size, tile_size});

    for (uint i = 0; i < _data.size(); ++i) {
      for (uint j = 0; j < _data[i].size(); ++j) {
        // clang-format off
        if ( // cpu view culling
          (i + 1) * tile_size  < view.left()   ||
          view.right()         < i * tile_size ||
          (j + 1) * tile_size  < view.bottom() || 
          view.top()           < j * tile_size 
        ) continue;
        // clang-format on

        rects.add()
          .color(TileProperties::of(_data[i][j]).color)
          .position({i * tile_size, j * tile_size});
      }
    }

    rects.draw(view);
  }
};