#include "World.h"
#include "Graphics.h"
#include "Unit.h"

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

void World::_drawRegion(View& view) const {
  const glm::vec4 grassCol(.3, .6, .2, 1);
  const glm::vec4 waterCol(.1, .3, .8, 1);
  RectangleBatch rects;
  rects.size({tile_size, tile_size});

  for (int i = 0; i < world_size; ++i) {
    for (int j = 0; j < world_size; ++j) {
      // clang-format off
      if ( // cpu view culling
        (i + 1) * tile_size  < view.left()   ||
        view.right()         < i * tile_size ||
        (j + 1) * tile_size  < view.bottom() || 
        view.top()           < j * tile_size 
      ) continue;
      // clang-format on

      rects.add()
        .color(_region[i][j] == Tile::GRASS ? grassCol : waterCol)
        .position({i * tile_size, j * tile_size});
    }
  }

  rects.draw(view);
}

void World::_drawUnits(View& view) const {
  const glm::vec4 selectedCol{1, 1, 0, 1}, unselectedCol{1, 0, 0, 1};
  CircleBatch circles;
  circles.size({tile_size, tile_size});

  for (auto& u : _units) {
    circles.add()
      .position(u.pos())
      .color(u.selected() ? selectedCol : unselectedCol);
  }

  circles.draw(view);
}
