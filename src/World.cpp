#include "World.h"
#include "Graphics.h"
#include "Unit.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void mat4_print(glm::mat4 m) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::cout << m[j][i] << "  ";
    }
    std::cout << std::endl;
  }
}

void World::_drawRegion(View& view) const {
  std::vector<glm::vec2> grass;
  std::vector<glm::vec2> water;

  for (int i = 0; i < world_size; ++i) {
    for (int j = 0; j < world_size; ++j) {
      if ( // cpu view culling
        (i + 1) * tile_size  < view.left()   ||
        view.right()         < i * tile_size ||
        (j + 1) * tile_size  < view.bottom() || 
        view.top()           < j * tile_size 
      ) continue;
      if (_region[i][j] == Tile::GRASS) grass.emplace_back(i * tile_size, j * tile_size);
      if (_region[i][j] == Tile::WATER) water.emplace_back(i * tile_size, j * tile_size);      
    }
  }

  {
    InstancedArrayRectangle r(grass);
    r.size(tile_size, tile_size);
    r.color(.3, .6, .2);
    r.draw(view);
  }


  {
    InstancedArrayRectangle r(water);
    r.size(tile_size, tile_size);
    r.color(.1, .3, .8);
    r.draw(view);
  }

  // InstancedRectangle r;
  // r.size(tile_size, tile_size);
  // r.color(.3, .6, .2);
  // r.positions(grass);
  // r.draw(view);

  // r.color(.1, .3, .8);
  // r.positions(water);
  // r.draw(view);
}

// void World::_drawUnits(sf::RenderTarget& window) const {
//   for (const Unit& u : _units) { window.draw(u); }
// }